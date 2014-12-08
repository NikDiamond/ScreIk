#ifndef AREASCREEN_H
#define AREASCREEN_H

#include <QWidget>
#include <QObject>
#include <QCursor>
#include <QDebug>
#include <QPoint>
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>

#include "areahook.h"

class AreaScreen : public QWidget
{
    Q_OBJECT
private:
    AreaHook *hooker;
    bool isPressed;
    QPoint startPosition;
    QPoint endPosition;
    int x;
    int y;
    int w;
    int h;
public:
    explicit AreaScreen(QWidget *parent = 0);
    ~AreaScreen();
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void hookEvent(bool pr, QPoint pt);
    void moveEvent(QPoint pt);
signals:
    void completed(int,int,int,int);
};

#endif // AREASCREEN_H
