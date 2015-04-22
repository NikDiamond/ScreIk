#include "siteopen.h"

SiteOpen::SiteOpen(QString email, QString password, QObject *parent) :
    QObject(parent)
{
    QUrl url("http://"+GLOBAL::domain+"/app.php?a=getAuthKey");
    QByteArray request(QString("email=" + email + "&password=" + password).toLocal8Bit());

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, GLOBAL::userAgent);

    reply = manager.post(req, request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(keyGot()));
}

SiteOpen::~SiteOpen()
{
    delete reply;
}

void SiteOpen::use()
{}

void SiteOpen::keyGot()
{
    QString rp = reply->readAll();
    qDebug() << "Key: " << rp;
    if(rp != "auth-error"){
        QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/?p=login&key="+rp));
    }else{
        qDebug() << rp;
    }
    this->deleteLater();
}
