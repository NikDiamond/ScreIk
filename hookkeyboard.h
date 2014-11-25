#ifndef HOOKKEYBOARD_H
#define HOOKKEYBOARD_H

#include <QObject>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <Windows.h>
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

class HookKeyboard : public QObject
{
    Q_OBJECT
private:
    HookKeyboard(QObject *parent = 0);
    static HookKeyboard *m_instance;
    HHOOK hhkLowLevelKybd;
    bool m_start;
public:
    static HookKeyboard *instance();
    enum HookKey {Print = 0,
                  Shift,
                  Ctrl
                  };
    void startHook();
    void endHook();
    bool isActive(){return m_start;}
public slots:
    void press(int);
signals:
    void keyPress(HookKeyboard::HookKey);
};

#endif // HOOKKEYBOARD_H
