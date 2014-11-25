#include "hookkeyboard.h"
#include <QDebug>


HookKeyboard *HookKeyboard::m_instance = NULL;

HookKeyboard::HookKeyboard(QObject *parent) : QObject(parent)
{
    m_start = false;
}

HookKeyboard *HookKeyboard::instance()
{
    if (m_instance == NULL)
        m_instance = new HookKeyboard();
    return m_instance;
}


void HookKeyboard::startHook()

{
    hhkLowLevelKybd =  SetWindowsHookEx(WH_KEYBOARD_LL, (LowLevelKeyboardProc), 0, 0);
    m_start = true;
}


void HookKeyboard::endHook()
{
    UnhookWindowsHookEx(hhkLowLevelKybd);
    m_start = false;
}


void HookKeyboard::press(int key)
{
    HookKeyboard::HookKey k = static_cast<HookKeyboard::HookKey>(key);
    emit keyPress(k);
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   BOOL fEatKeystroke = FALSE;

   if (nCode == HC_ACTION) {
      switch (wParam) {
      case WM_KEYDOWN:  case WM_SYSKEYDOWN:
         PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;

         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_CONTROL) & 0x8000) != 0)) && (((GetKeyState(VK_SHIFT) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeyboard::instance()->press(HookKeyboard::Print | HookKeyboard::Ctrl | HookKeyboard::Shift);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }

         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_CONTROL) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeyboard::instance()->press(HookKeyboard::Print | HookKeyboard::Ctrl);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));

         }

         fEatKeystroke = ((p->vkCode == VK_SNAPSHOT) && (((GetKeyState(VK_SHIFT) & 0x8000) != 0)));
         if (fEatKeystroke)
         {
             HookKeyboard::instance()->press(HookKeyboard::Print | HookKeyboard::Shift);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }

         fEatKeystroke=(p->vkCode == VK_SNAPSHOT);
         if (fEatKeystroke)
         {
             HookKeyboard::instance()->press(HookKeyboard::Print);
             return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
         }
         break;
      }
   }
   return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}
