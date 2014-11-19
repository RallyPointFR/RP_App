#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

class SplashScreen;

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QShowEvent>
#include <QCloseEvent>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLineEdit>
#include <QTime>
#include <QMouseEvent>
#include <QFocusEvent>

#include "GameList.h"
#include "mail.h"

class SplashScreen : public QWidget
{
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = 0); //Constructeur
    ~SplashScreen(); //Destructeur


    static const QString pwd;  //Variable static constante pour stoquer le mot de passe
    static bool    verifPwd(); //methode statique pour vérifier le mot de passe

protected:
    void showEvent(QShowEvent* e);  //Evenement d'affichage
    void closeEvent(QCloseEvent* e);//Evenement de fermeture
    void enterEvent(QEvent* e);     //Evenement de fenetre active

private slots:
    void slot_pref();               //Slot de bouton pref
    void slot_hover(QString desc);  //Slot de selection d'un jeu
    void slot_timer(QString nom);   //Slot de lancement de timer
    void slot_email();              //Slot d'envois d'un email

private:
    void    reloadGames(); //Rechargement de la liste de jeu

    QLabel* bg;     //img Background
    QLabel* logo;   //img logo
    QLabel* titre;  //texte titre

    QFrame* frameDesc;  //frame à gauche
    QLabel* desc;       //Afficheur de description de jeu
    mail*   email;      //widget email
    QPushButton* go;    //bouton d'envois de l'addr email

    QPushButton* prefButton;    //Bouton pref
    QPushButton* closeButton;   //Bouton close

    GameList* list;             //Liste des jeu

    QTime   timer;      //Timer de temps de jeu
    QString lastName;   //Nom du jeu en cours d'exec
};

#endif // SPLASHSCREEN_H
