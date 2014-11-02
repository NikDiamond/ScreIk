#ifndef AREACLASS_H
#define AREACLASS_H

#include <QObject>
#include <QWidget>
#include <QShortcut>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

class AreaClass : public QWidget
{
    Q_OBJECT
private:
    QShortcut *shortcut;
    QPoint startPosition;
    QPoint endPosition;
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
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void completed(int,int,int,int);
};

#endif // AREACLASS_H
