#include <QSettings>
#include <QDebug>
#include "GameList.h"

GameList::GameList(QWidget *parent) :
    QWidget (parent),
    layout  (0)
{
}

//Recharge la liste de jeux
void GameList::reloadGames()
{
    //Si il y a déjà un liste en place, on la supprime
    if(layout != 0)
    {
        for(register int i = 0; i < list.length(); i++)
        {
            disconnect(list[i], SIGNAL(hover(QString)), this, SLOT(receive_hover(QString)));
            layout->removeWidget(list[i]);
            delete list[i];
        }
        delete layout;
    }

    list.clear();

    //On créer une nouvelle liste
    layout = new QGridLayout;

    setLayout(layout);

    //récupération de la liste de jeu dans le fichier ini
    QSettings pref("pref.ini", QSettings::IniFormat);

    QStringList listGames(pref.childGroups());

    //Variavle de ligne et de colone
    int l   (0);
    int c   (0);

    //Pour chaque jeu trouvé
    for(register int i = 0; i < listGames.length(); i++)
    {
        //On créer un nouveau bouton
        GameButton* newButton(new GameButton(scrWidth,
                                             listGames[i],
                                             pref.value(listGames[i] + "/desc").toString(),
                                             pref.value(listGames[i] + "/path").toString(),
                                             pref.value(listGames[i] + "/pwd").toBool(),
                                             pref.value(listGames[i] + "/steam").toBool(),
                                             pref.value(listGames[i] + "/gameID").toString()));
        //On l'ajoute à la liste
        list.append(newButton);

        //Connexion des slots
        connect(newButton, SIGNAL(hover(QString, QString)), this, SLOT(receive_hover(QString, QString)));
        connect(newButton, SIGNAL(launch(QString)), this, SLOT(receive_lauch(QString)));

        //Affichage
        layout->addWidget(newButton, l, c, Qt::AlignCenter | Qt::AlignHCenter);

        c++;

        if(c >= 10)
        {
            c = 0;
            l++;
        }
    }
}

//Slot quand un jeu est selectionné
void GameList::receive_hover(QString nom, QString desc)
{
    //On dé-check les autres bouton si il y en a
    for(register int i = 0; i < list.length(); i++)
        list[i]->unchecked(nom);
    emit buttonFocus(desc); //On envois la description à la fenetre principale
}

//Quand on veut lancer un jeu
void GameList::receive_lauch(QString nom)
{
    emit lauch(nom); //On envois un signal de lancement (qui va lancer le timer toussa toussa...
}
