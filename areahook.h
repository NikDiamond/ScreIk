#ifndef AREACLASS_H
#define AREACLASS_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QCursor>
#include <QPoint>

#include <Windows.h>
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

class AreaHook : public QObject
{
    Q_OBJECT
private:
    AreaHook(QObject *parent = 0);
    ~AreaHook();
    static AreaHook *m_instance;
    HHOOK hhkLowLevelKybd;
    bool m_start;
    QWidget *wCursor;
public:
    static AreaHook *instance();
    void endHook();
    void startHook();
public slots:
    void press(bool pressed, QPoint point = QPoint());
    void moved(QPoint point);
signals:
    void mEvent(bool, QPoint);
    void mMoved(QPoint);
};

#endif // AREACLASS_H
