#include <QApplication>
#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include <QMovie>

#include "global.h"
#include "QLogger.h"
#include "SplashScreen.h"
#include "SaisiePwd.h"

const QString SplashScreen::pwd = QString("RallyPointA2P2014");

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget     (parent),
    bg          (new QLabel(this)),
    logo        (new QLabel(this)),
    titre       (new QLabel(this)),
    frameDesc   (new QFrame(this)),
    desc        (new QLabel(frameDesc)),
    email       (new mail(frameDesc)),
    go          (new QPushButton("Laisser l'E-mail", frameDesc)),
    prefButton  (new QPushButton("", this)),
    closeButton (new QPushButton("", this)),
    list        (new GameList(this))
{
    //Définition de la fenetre principale
    setMinimumSize(1024, 768);
    setWindowIcon(QIcon(":/img/logo.png"));
    setMouseTracking(true);

    //bakcground
    bg->setPixmap(QPixmap(":/img/bg.jpg"));
    bg->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

    //Logo & Titre
    logo->setPixmap(QPixmap(":/img/logo.png").scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    titre->setText("Rally Point Launcher");
    titre->setFont(QFont("Arial", 16, QFont::Bold, true));
    titre->setStyleSheet("color:white;");
    titre->setAlignment(Qt::AlignHCenter);

    //espace d'affichage de la description
    QLabel* titreDesc(new QLabel("Description"));
    titreDesc->setFont(QFont("Arial", 12, QFont::Bold));
    titreDesc->setMaximumHeight(20);

    //label pour l'email
    QLabel* LB_mail(new QLabel("Laissez nous votre e-mail :"));
    LB_mail->setFont(QFont("Arial", 12));
    LB_mail->setMaximumHeight(20);

    //La frame à gauche regroupant description + mail
    frameDesc->setStyleSheet("background-color:rgba(64, 64, 64, 100);");
    frameDesc->setLayout(new QVBoxLayout);
    frameDesc->layout()->addWidget(titreDesc);
    frameDesc->layout()->addWidget(desc);
    frameDesc->layout()->addWidget(LB_mail);
    frameDesc->layout()->addWidget(email);
    frameDesc->layout()->addWidget(go);

    desc->setStyleSheet("background-color:rgba(0, 0, 0, 0);color:white;");
    desc->setAlignment(Qt::AlignJustify);
    desc->setWordWrap(true);
    desc->setFont(QFont("Arial", 12));

    email->setStyleSheet("background-color:white;");
    go->setStyleSheet("background-color:rgba(72, 72, 72, 255);color:white;");
    LB_mail->setStyleSheet("background-color:rgba(0, 0, 0, 0);color:white;");
    titreDesc->setStyleSheet("background-color:rgba(0, 0, 0, 0);color:#404040;");

    //Bouton préférence
    prefButton->setIconSize(QSize(24, 24));
    prefButton->setIcon(QIcon(":/img/pref.png"));
    prefButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    prefButton->setFlat(true);

    //Bouton close
    closeButton->setIconSize(QSize(24, 24));
    closeButton->setIcon(QIcon(":/img/close.png"));
    closeButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    closeButton->setFlat(true);

    //Connexion signaux / slots
    connect(prefButton, SIGNAL(clicked()), this, SLOT(slot_pref()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(list, SIGNAL(buttonFocus(QString)), this, SLOT(slot_hover(QString)));
    connect(list, SIGNAL(lauch(QString)), this, SLOT(slot_timer(QString)));
    connect(go, SIGNAL(clicked()), this, SLOT(slot_email()));
}

SplashScreen::~SplashScreen()
{
}

//Evenement déclenché lors de l'affichage de la fenetre, on place nos élément à ce moment là en foction de la réso
void SplashScreen::showEvent(QShowEvent* e)
{
    e->accept();

    logo->setGeometry(20, 20, 100, 100);
    titre->setGeometry(0, 50, width(), titre->height());

    frameDesc->setGeometry(20, 200, width()/4.66f, height()-220);

    list->setScrWidth(width());
    list->setGeometry(width()/4.66f + 40, 200, width() - (width()/4.66f + 40), height() -220);

    prefButton->move(width() - 2*prefButton->width() - 40, 20);
    closeButton->move(width() - prefButton->width() - 20, 20);

    //Et on en profite pour recharger la liste de jeux
    reloadGames();
}

//Evenement de fermeture : check du mdp
void SplashScreen::closeEvent(QCloseEvent* e)
{
    //*
    if(verifPwd())
        e->accept();
    else
        e->ignore();
    /*/
    e->accept();
    //*/
}

//Evenement déclenché quand la fenetre redevient active
//C'est ici qu'on calcul les stats d'un jeu et qu'on re-capture le clavier
void SplashScreen::enterEvent(QEvent* e)
{
    //si le jeu a été lancé plus de 10 seconde (pour palier au porblème de steam ou des launcher
    if(timer.elapsed() > 10000)
    {
        //Si le clavier n'est pas capturé
        if(!isHook())
        {
            //On le capture et si ça marche pas on affiche une erreur
            if(!hookKB())
                QMessageBox::warning(this, "Erreur", "Le clavier n'a pas été capturé...");
        }

        //Si il y a eu un jeu de lancé
        if(!lastName.isEmpty())
        {
            //On calcul ses stats
            int ms = timer.elapsed();
            QLogger::QLog_Info("game", "Info : " + lastName + " a tourné pendant " + QVariant(ms/1000).toString() + "s");

            QSettings pref("stats/" + lastName + ".ini", QSettings::IniFormat);
            int sec = pref.value("time", QVariant(0)).toInt();
            pref.setValue("time", QVariant(sec + ms/1000));

            lastName.clear();
        }
    }
    e->accept();
}

//Appuis sur le bouton pref
void SplashScreen::slot_pref()
{
    //Verif du mdp
    if(verifPwd())
    {   
        //Débloquage du clavier
        if(unhookKB())
        {
            //Lancement de RPAssistant
            QProcess proc;

            proc.start("RPAssistant.exe");

            //Attente de la fin d'exécution
            proc.waitForFinished(-1);

            //Rechargement de la liste
            reloadGames();

            //Recapture du clavier (si ça marche pas : erreur)
            if(!hookKB())
                QMessageBox::warning(this, "Erreur", "Le clavier n'a pas été capturé...");
        }
    }
}

//Affichage de la description quand on selectionne un jeu
void SplashScreen::slot_hover(QString desc)
{
    this->desc->setText(desc);
}

//Lancement du timer au lancement d'un jeu (pour les stats)
void SplashScreen::slot_timer(QString nom)
{
    QLogger::QLog_Info("game", "Info : TimerStart " + nom);
    lastName = nom;
    timer.start();
}

//Ajout de l'email entré
void SplashScreen::slot_email()
{
    QStringList listMail;

    QSettings pref("pref.ini", QSettings::IniFormat);
    listMail = pref.value("email", QVariant(QStringList())).toStringList();

    listMail.append(email->getMail());

    pref.setValue("email", QVariant(listMail));

    QLogger::QLog_Info("interface", "Info : mail enregistré " + email->getMail());

    email->clear();

    QMessageBox::information(this, "Email enregistré", "Merci! :)");
}

//Varification du mdp
bool SplashScreen::verifPwd()
{
    bool sortieBoucle(false);
    bool incorrect(false);
    bool valider(false);

    //On boucle
    while(!sortieBoucle)
    {
        //Lancement de la fenetre de saisie
        SaisiePwd saisie(incorrect);

        //Si on a cliqué sur ok
        if(saisie.exec() == QDialog::Accepted)
        {
            //Si le mdp est le bon
            if(saisie.getPwd() == SplashScreen::pwd)
            {
                //On sort de la boucle et on valide
                sortieBoucle = true;
                valider = true;
            }
            else
            {
                //Sinon on renvois incorrect (pour afficher un message en rouge dans la fenetre de saisie)
                incorrect = true;
            }
        }
        else //Sinon si on clic sur annulé, on sort simplement la boucle
        {
            sortieBoucle = true;
        }
    }

    return valider; //On renvois la validation
}

//Reload de la list de jeu
void SplashScreen::reloadGames()
{
    QLogger::QLog_Info("interface", "Info : Reload de la liste");
    list->reloadGames();
}
