#include <QApplication>
#include <QGuiApplication>
#include <QMessageBox>
#include <QProcess>

#include "global.h"
#include "QLogger.h"
#include "SplashScreen.h"

int main(int argc, char *argv[])
{
    //Déclaration de l'application Qt
    QApplication a(argc, argv);

    //Init du système de log
    QStringList module;
    module << "keyboard" << "game" << "interface";

    QLogger::QLoggerManager* manager = QLogger::QLoggerManager::getInstance();
    manager->addDestination("log.txt",module , QLogger::InfoLevel);

    //Capture du clavier
    if(!hookKB())
        QMessageBox::warning(0, "Erreur", "Erreur, la capture du clavier a échouée...");

    //Déclaration et affichage de la fenetre principale
    SplashScreen w;
    w.showFullScreen();

    //execution de l'appli
    int retour(a.exec());

    //Relache du clavier
    if(!unhookKB())
        QMessageBox::critical(0, "Erreur", "Le clavier n'a pas put être relaché!");

    return retour;
}
