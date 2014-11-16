#include "loginclass.h"

logInClass::logInClass(QString email, QString password, QObject *parent) :
    QObject(parent), email(email), pass(password)
{}
logInClass::~logInClass()
{
    delete reply;
}

void logInClass::tryLogin()
{
    QUrl url("http://nikdiamond.hol.es/login.php");
    QByteArray request(QString("email=" + email + "&password=" + pass).toLocal8Bit());

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "ScreeIk_1.0");
    reply = manager.post(req, request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void logInClass::readyRead()
{
    QString rp = reply->readAll();
    emit gotReply(rp);
    deleteLater();
}
