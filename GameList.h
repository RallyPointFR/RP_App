#ifndef GAMELIST_H
#define GAMELIST_H

class GameList;

#include <QWidget>
#include <QGridLayout>
#include "GameButton.h"

class GameList : public QWidget
{
    Q_OBJECT
public:
    explicit GameList(QWidget *parent = 0);

    void reloadGames(); //Recharge la liste

    inline void setScrWidth(int w) { scrWidth = w;} //setter pour la largeur de l'écran

signals:
    void buttonFocus(QString desc); //Signal quand on selectionne un jeu
    void lauch(QString nom);        //Signal quand on lance un jeu

private slots:
    void receive_hover(QString nom, QString desc);  //Slot quand on selectionne un jeu
    void receive_lauch(QString nom);                //Slot quand on lance un jeu

private:
    QGridLayout* layout; //Layout d'affichage
    int scrWidth;        //Largeur de l'écran

    QList<GameButton*> list; //Liste de jeux
};

#endif // GAMELIST_H
