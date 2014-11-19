#-------------------------------------------------
#
# Project created by QtCreator 2014-10-18T11:35:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RP_App
TEMPLATE = app


SOURCES += main.cpp \
    SplashScreen.cpp \
    GameList.cpp \
    GameButton.cpp \
    SaisiePwd.cpp \
    global.cpp \
    mail.cpp \
    QLogger.cpp

HEADERS  += \
    SplashScreen.h \
    GameList.h \
    GameButton.h \
    SaisiePwd.h \
    global.h \
    mail.h \
    QLogger.h

FORMS    += \
    Pref.ui \
    SaisiePwd.ui \
    mail.ui

RESOURCES += \
    ressources.qrc
