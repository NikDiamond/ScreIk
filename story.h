#ifndef STORY_H
#define STORY_H

#include "global.h"

#include <QObject>
#include <QList>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class story : public QObject
{
    Q_OBJECT
public:
    explicit story(QObject *parent = 0);
    void getElements(QString email, QString password, int length);
    ~story();

private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
private slots:
    void parse();
signals:
    void finished(QStringList);
};

#endif // STORY_H
