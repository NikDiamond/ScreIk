#ifndef LOGINCLASS_H
#define LOGINCLASS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QUrl>

class logInClass : public QObject
{
    Q_OBJECT
public:
    explicit logInClass(QString email, QString password, QObject *parent = 0);
    void tryLogin();
    ~logInClass();
private:
    QNetworkReply *reply;
    QNetworkAccessManager manager;
    QString email;
    QString pass;
private slots:
    void readyRead();
signals:
    void gotReply(QString);
};

#endif // LOGINCLASS_H
