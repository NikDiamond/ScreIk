#include "story.h"

story::story(QObject *parent) : QObject(parent)
{}

void story::getElements(QString email, QString password, int length)
{
    QUrl url("http://"+GLOBAL::domain+"/app.php?a=getStory");
    QByteArray request(QString("email=" + email + "&password=" + password + "&length=" + length).toLocal8Bit());

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, GLOBAL::userAgent);

    reply = manager.post(req, request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(parse()));
}
story::~story()
{
    delete reply;
}

void story::parse()
{
    QString rep = reply->readAll();
    rep = rep.simplified();
    QStringList screens = rep.split('|');
    emit finished(screens);
}

