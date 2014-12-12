#include "areahook.h"

AreaHook *AreaHook::m_instance = NULL;

AreaHook::AreaHook(QObject *parent) :
    QObject(parent)
{
    m_start = false;
}

void AreaHook::deleteNow()
{
    wCursor->hide();
    wCursor->deleteLater();
}

AreaHook::~AreaHook()
{
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    switch( wParam )
    {
        case WM_MOUSEMOVE:
            AreaHook::instance()->moved(QCursor::pos());
            return 0;
            break;
        case WM_LBUTTONDOWN: case WM_RBUTTONDOWN:
            AreaHook::instance()->press(true, QCursor::pos());
            return 1;
            break;
        case WM_LBUTTONUP: case WM_RBUTTONUP:
            AreaHook::instance()->press(false, QCursor::pos());
            return 0;
            break;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

AreaHook *AreaHook::instance()
{
    if (m_instance == NULL)
        m_instance = new AreaHook();
    return m_instance;
}


void AreaHook::startHook()
{
    wCursor = new QWidget();
    wCursor->setWindowFlags(wCursor->windowFlags() | Qt::Tool);
    wCursor->setWindowFlags(wCursor->windowFlags() | Qt::WindowStaysOnTopHint);
    wCursor->setWindowFlags(wCursor->windowFlags() | Qt::FramelessWindowHint);
    wCursor->setWindowOpacity(.01);
    wCursor->setCursor(Qt::CrossCursor);
    wCursor->setGeometry(QCursor::pos().x()-5, QCursor::pos().y()-5, 10, 10);
    wCursor->show();
    hhkLowLevelKybd =  SetWindowsHookEx(WH_MOUSE_LL, (MouseProc), 0, 0);
    m_start = true;
}


void AreaHook::endHook()
{
    UnhookWindowsHookEx(hhkLowLevelKybd);
    m_start = false;
}


void AreaHook::press(bool pressed, QPoint point)
{
    if(wCursor->isVisible()){
        wCursor->hide();
    }
    emit mEvent(pressed, point);
    if(!pressed)
        m_instance = NULL;
}

void AreaHook::moved(QPoint point)
{
    if(wCursor->isVisible())
        wCursor->setGeometry(point.x()-5, point.y()-5, 10, 10);
    emit mMoved(point);
}

