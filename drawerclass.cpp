#include "drawerclass.h"

DrawerClass::DrawerClass(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(windowFlags() | Qt::Tool);
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setWindowOpacity(.2);
}

void DrawerClass::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainter p(this);
    QPen pen;
    pen.setWidth(2);

    p.setPen(pen);
    p.setBackground(QBrush(QColor(0,0,0),Qt::SolidPattern));
    p.drawRect(1, 1, this->width()-2, this->height()-2);
}
