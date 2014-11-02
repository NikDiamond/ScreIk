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
    if(startPosition.x() < event->pos().x() || startPosition.y() < event->pos().y()){
        x = startPosition.x();
        y = startPosition.y();
        w = event->pos().x() - x;
        h = event->pos().y() - y;
    }else{
        x = event->pos().x();
        y = event->pos().y();
        w = startPosition.x() - x;
        h = startPosition.y() - y;
    }
    this->repaint();
}

void AreaClass::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    endPosition = event->pos();
    if(startPosition.x() < endPosition.x()){
        x = startPosition.x();
        w = endPosition.x() - startPosition.x();
    }else{
        x = endPosition.x();
        w = startPosition.x() - endPosition.x();
    }

    if(startPosition.y() < endPosition.y()){
        y = startPosition.y();
        h = endPosition.y() - startPosition.y();
    }else{
        y = endPosition.y();
        h = startPosition.y() - endPosition.y();
    }

    emit completed(x,y,w,h);
    emit shortcut->activated();
}

void AreaClass::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainter p(this);
    p.drawRect(x, y, w, h);
}
