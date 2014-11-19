#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "global.h"
#include "QLogger.h"
#include "GameButton.h"
#include "SplashScreen.h"

GameButton::GameButton(int width, QString nom, QString desc, QString path, bool pwd, bool steam, QString gameID, QWidget *parent) :
    QCommandLinkButton  (parent),
    nom                 (nom),
    desc                (desc),
    path                (path),
    pwd                 (pwd),
    steam               (steam),
    gameID              (gameID),
    proc                (new QProcess(this))
{
    //Définition de l'apparence du bouton
    setMaximumSize(width / 21.875f, (width / 21.875f));
    setIconSize(QSize(maximumWidth()-16, maximumHeight()-16));
    setIcon(QIcon("./icons/" + nom + ".png"));
    setCheckable(true);

    //On définit le working directory du jeu représenté
    proc->setWorkingDirectory(QFileInfo(path).absolutePath());

    //connexion signaux/slots
    connect(this, SIGNAL(clicked()), this, SLOT(click()));
    connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
    connect(proc, SIGNAL(started()), this, SLOT(started()));
    connect(proc, SIGNAL(finished(int)), this, SLOT(finished(int)));
}

//Event de double click
void GameButton::mouseDoubleClickEvent(QMouseEvent* e)
{
    e->accept();

    bool lauchGame(true); //Représente si le jeu peut être lancé ou non

    if(pwd) //Si verif de mdp alors verif de mdp
        lauchGame = SplashScreen::verifPwd();

    //Si le jeu peut être lancé
    if(lauchGame)
    {
        QLogger::QLog_Info("game", "Info : Tentative de lancement : " + path);

        //Si on réussis à relacher le clavier
        if(unhookKB())
        {
            //On update certaine stats
            QSettings* pref(new QSettings("stats/" + nom + ".ini", QSettings::IniFormat));

            int val(pref->value("launch", QVariant(0)).toInt());
            pref->setValue("launch", QVariant(val+1));
            delete pref;


            //Si c'est un jeu steam, on va chercher l'exe steam qu'on lance avec certain arguments
            if(steam)
                proc->start(QSettings("pref.ini", QSettings::IniFormat).value("steamPath").toString(), QStringList() << "-applaunch" << gameID);
            else //Sinon on lance le jeu tout simplement
                proc->start(path);
        }
        else
            QMessageBox::warning(0, "Erreur", "Le système refuse de relâcher la capture clavier");
    }
}

//Si on click sur le jeu, on envoit le signal de selection à la liste
void GameButton::click()
{
    emit hover(nom, desc);
}

//Gestion du log pour les erreur de lancement
void GameButton::error(QProcess::ProcessError err)
{
    switch(err)
    {
    case QProcess::FailedToStart :
        QLogger::QLog_Error("game", "Error : Fail To Start");
        break;

    case QProcess::Crashed :
        QLogger::QLog_Error("game", "Error : Crashed");;
        break;

    case QProcess::Timedout :
        QLogger::QLog_Error("game", "Error : Timed out");
        break;

    case QProcess::WriteError :
        QLogger::QLog_Error("game", "Error : Write Error");
        break;

    case QProcess::ReadError :
        QLogger::QLog_Error("game", "Error : Read Error");
        break;

    case QProcess::UnknownError :
        QLogger::QLog_Error("game", "Error : Unknown Error");
        break;
    }
}

//Signal quand le jeu est lancé
void GameButton::started()
{
    //On log, on emet le signal de lancement
    QLogger::QLog_Info("game", "Info : Commande lancée pour le jeu : " + nom);
    emit launch(nom);

    //On attend la fin de l'executions
    proc->waitForFinished(-1);
}

//Signal quand l'exec est terminée, on log
void GameButton::finished(int code)
{
    QLogger::QLog_Info("game", "Info : Commande terminée pour le jeu : " + nom + " avec le code " + QVariant(code).toString());
}

//Si le nom en parametre est différent du notre, on se déselectionne.
void GameButton::unchecked(QString nom)
{
    if(nom != this->nom)
        setChecked(false);
}
