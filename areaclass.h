#ifndef AREACLASS_H
#define AREACLASS_H

#include "drawerclass.h"
#include <QObject>
#include <QWidget>
#include <QShortcut>
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QApplication>
#include <QDesktopWidget>

class AreaClass : public QWidget
{
    Q_OBJECT
private:
    QShortcut *shortcut;
    QShortcut *shortcutDr;
    QPoint startPosition;
    QPoint endPosition;
    DrawerClass *drawer;
    int x;
    int y;
    int w;
    int h;
public:
    explicit AreaClass(QWidget *parent = 0);
    ~AreaClass();
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
signals:
    void completed(int,int,int,int);
};

#endif // AREACLASS_H
