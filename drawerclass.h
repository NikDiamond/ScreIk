#ifndef DRAWERCLASS_H
#define DRAWERCLASS_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPen>

class DrawerClass : public QWidget
{
    Q_OBJECT
public:
    explicit DrawerClass(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
};

#endif // DRAWERCLASS_H
