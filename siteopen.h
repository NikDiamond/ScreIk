#ifndef SITEOPEN_H
#define SITEOPEN_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QDesktopServices>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class SiteOpen : public QObject
{
    Q_OBJECT
public:
    explicit SiteOpen(QString email, QString password, QObject *parent = 0);
private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
private slots:
    void keyGot();
};

#endif // SITEOPEN_H
