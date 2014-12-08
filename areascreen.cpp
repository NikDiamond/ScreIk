#include "areascreen.h"

AreaScreen::AreaScreen(QWidget *parent) :
    QWidget(parent), isPressed(false)
{
    this->setWindowFlags(windowFlags() | Qt::Tool);
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setWindowOpacity(.2);

    hooker = AreaHook::instance();
    hooker->startHook();

    connect(hooker, SIGNAL(mEvent(bool,QPoint)), this, SLOT(hookEvent(bool,QPoint)));
    connect(hooker, SIGNAL(mMoved(QPoint)), this, SLOT(moveEvent(QPoint)));
}

AreaScreen::~AreaScreen()
{
    hooker->deleteLater();
}

void AreaScreen::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainter p(this);
    QPen pen;
    pen.setWidth(2);

    p.setPen(pen);
    p.setBackground(QBrush(QColor(0,0,0),Qt::SolidPattern));
    p.drawRect(1, 1, this->width()-2, this->height()-2);
}

void AreaScreen::hookEvent(bool pr, QPoint pt)
{
    if(pr){//just pressed
        startPosition = pt;
        isPressed = true;
    }else{//set end point
        endPosition = pt;
        isPressed = false;
        hooker->endHook();
        //Screening
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
        this->setGeometry(0,0,0,0);
        if(this->isVisible()){
            this->hide();
        }
        emit completed(x,y,w,h);
        //
    }
}

void AreaScreen::moveEvent(QPoint pt)
{
    if(isPressed){//if pressed now
        if(startPosition.x() < pt.x()){
            x = startPosition.x();
            w = pt.x() - startPosition.x();
        }else{
            x = pt.x();
            w = startPosition.x() - pt.x();
        }
        if(startPosition.y() < pt.y()){
            y = startPosition.y();
            h = pt.y() - startPosition.y();
        }else{
            y = pt.y();
            h = startPosition.y() - pt.y();
        }

        this->setGeometry(x,y,w,h);
        if(!this->isVisible())
            this->show();
    }
}
