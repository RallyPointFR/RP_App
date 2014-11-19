#ifndef GLOBAL_H
#define GLOBAL_H
#include <QDebug>
#include <windows.h>
//fichier pour les fonction C globale au programme (capture du clavier)
extern HHOOK g_hKeyboardHook;   //Variable globale contenant le handle de la capture clavier
extern bool  hook;              //Booléen représentant si le clavier est capturé ou non

extern LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam); //Fonction de capure Win API
extern bool hookKB();   //Capture du clavier
extern bool isHook();   //Verifie si le clavier est capturé ou non
extern bool unhookKB(); //Relache du clavier
#endif // GLOBAL_H
