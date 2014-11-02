#include "areaclass.h"

AreaClass::AreaClass(QWidget *parent) :
    QWidget(parent)
{
    shortcut = new QShortcut(Qt::Key_Escape, this);
    this->setWindowOpacity(0.1);
    this->setStyleSheet("QWidget{background: #fff}");
    this->setCursor(Qt::CrossCursor);
    this->showFullScreen();

    connect(shortcut, SIGNAL(activated()), shortcut, SLOT(deleteLater()));
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteLater()));

}

AreaClass::~AreaClass()
{}

void AreaClass::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    startPosition = event->pos();
}

void AreaClass::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void AreaClass::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    endPosition = event->pos();
    if(startPosition.x() < endPosition.x() || startPosition.y() < endPosition.y()){
        x = startPosition.x();
        y = startPosition.y();
        w = endPosition.x() - x;
        h = endPosition.y() - y;
    }else{
        x = endPosition.x();
        y = endPosition.y();
        w = startPosition.x() - x;
        h = startPosition.y() - y;
        qDebug() << x << y << w << h;
    }

    emit completed(x,y,w,h);
    emit shortcut->activated();
}
