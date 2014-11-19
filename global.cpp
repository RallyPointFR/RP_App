#include "global.h"
#include "QLogger.h"

//Définition des variables globales
HHOOK g_hKeyboardHook = 0;
bool  hook = false;

//Fonction WIN API
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 || nCode != HC_ACTION )  // Dans ce cas, on ne gère pas le message
        return CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam);

    //Dans le cas ou on le gère, on récupere la structure de l'event
    bool bEatKeystroke = false;
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

    //Si ce flag est présent ça veux dire que ALT est pressé donc on bouffe l'event
    if(p->flags == LLKHF_ALTDOWN)
    {
        bEatKeystroke = /*g_bWindowActive &&*/true;
    }
    else    //Sinon, si il s'agis d'une des touches suivante : on mange aussi
    {
        bEatKeystroke = (//g_bWindowActive &&
                         (
                         (p->vkCode == VK_LWIN) ||
                         (p->vkCode == VK_RWIN) ||
                         (p->vkCode == VK_LCONTROL) ||
                         (p->vkCode == VK_RCONTROL) ||
                         (p->vkCode == VK_MENU) ||
                         (p->vkCode == VK_ESCAPE) ||
                         (p->vkCode == VK_TAB) ||
                         (p->vkCode == VK_APPS) ||
                         (p->vkCode == VK_SLEEP)));
    }

    LRESULT retour;
    if( bEatKeystroke ) //Si on mange, on retourne 1
    {
        retour = 1;
        QLogger::QLog_Debug("keyboard", "Debug : ===Mange touche");
    }
    else //Sinon on envois renvois l'event au système
    {
        retour = CallNextHookEx( g_hKeyboardHook, nCode, wParam, lParam );
        QLogger::QLog_Debug("keyboard", "Debug : ===Passe touche");
    }

    return retour;
}

//Capture du clavier
bool hookKB()
{
    //Demande de handle
    g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,LowLevelKeyboardProc,GetModuleHandle(NULL),0);

    //Si on l'obtient
    if (g_hKeyboardHook != NULL)
    {
        QLogger::QLog_Info("keyboard", "Info : Capture clavier OK");
        hook = true;
    }
    else //Sinon
    {
        QLogger::QLog_Warning("keyboard", "Warning : Capture clavier NOK");
        hook = false;
    }

    return isHook(); //Retourne true si le clavier a correctement été capturé
}

bool isHook()
{
    return hook;
}

//Relache
bool unhookKB()
{
    //S'il est capturé
    if(hook)
    {
        //On le relache
        bool teste = UnhookWindowsHookEx(g_hKeyboardHook);

        //Si ça a marché
        if (teste)
        {
            QLogger::QLog_Info("keyboard", "Info : Relache clavier OK");
            hook = false;
        }
        else //sinon
        {
            QLogger::QLog_Warning("keyboard", "Warning : Relache clavier NOK");
            hook = true;
        }
    }

    return !isHook(); //Retourne true si le clavier à correctement été relaché
}
