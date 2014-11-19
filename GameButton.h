#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

class GameButton;

#include <QCommandLinkButton>
#include <QMouseEvent>
#include <QProcess>

class GameButton : public QCommandLinkButton
{
    Q_OBJECT
public:
    explicit GameButton(int width, QString nom, QString desc, QString path, bool pwd, bool steam = false, QString gameID = QString(), QWidget *parent = 0);

    void unchecked(QString nom); //Déselectionne si nom différent du notre
protected:
    void mouseDoubleClickEvent(QMouseEvent* e); //Event de double click

signals:
    void hover(QString nom, QString desc); //Signal de selection
    void launch(QString nom);              //Signal d'exec


private slots:
    void click(); //Slot de selection
    void error(QProcess::ProcessError err); //Slot de gestion d'erreur
    void started(); //Slot de lancement
    void finished(int code); //Slot de fin

private:
    QString     nom;
    QString     desc;
    QString     path;
    QIcon       icon;
    bool        pwd;
    bool        steam;
    QString     gameID;

    QProcess*   proc; //Gestionnaire de processus externe
};

#endif // GAMEBUTTON_H
