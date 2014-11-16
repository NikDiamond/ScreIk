#include "areaclass.h"

AreaClass::AreaClass(QWidget *parent) :
    QWidget(parent),x(0),y(0),w(0),h(0)
{
    this->setWindowFlags(windowFlags() | Qt::Tool);
    this->setStyleSheet("background: rgb(255,255,255)");
    this->setWindowOpacity(.01);
    this->setCursor(Qt::CrossCursor);
    this->showFullScreen();
    this->setFixedSize(this->size());

    drawer = new DrawerClass();
    shortcut = new QShortcut(Qt::Key_Escape, this);
    shortcutDr = new QShortcut(Qt::Key_Escape, drawer);

    connect(shortcut, SIGNAL(activated()), shortcut, SLOT(deleteLater()));
    connect(shortcut, SIGNAL(activated()), drawer, SLOT(deleteLater()));
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteLater()));

    connect(shortcutDr, SIGNAL(activated()), shortcut, SLOT(deleteLater()));
    connect(shortcutDr, SIGNAL(activated()), drawer, SLOT(deleteLater()));
    connect(shortcutDr, SIGNAL(activated()), this, SLOT(deleteLater()));
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
    if(startPosition.x() < event->pos().x()){
        x = startPosition.x();
        w = event->pos().x() - startPosition.x();
    }else{
        x = event->pos().x();
        w = startPosition.x() - event->pos().x();
    }

    if(startPosition.y() < event->pos().y()){
        y = startPosition.y();
        h = event->pos().y() - startPosition.y();
    }else{
        y = event->pos().y();
        h = startPosition.y() - event->pos().y();
    }
    drawer->setGeometry(x,y,w,h);
    drawer->show();
    drawer->repaint();
}

void AreaClass::mouseReleaseEvent(QMouseEvent *event)
{
    this->setWindowOpacity(0);
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

    drawer->setWindowOpacity(.001);
    emit completed(x,y,w,h);
    emit shortcut->activated();
}
