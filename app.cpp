#include "app.h"

App::App(QString act, QString POST, QObject *parent): QObject(parent)
{
    this->act = act;
    this->POST = POST;
    this->error = "";
}

void App::query()
{
    QUrl url("http://"+GLOBAL::domain+"/app.php?a="+act);
    QByteArray request(POST.toLocal8Bit());
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, GLOBAL::userAgent);

    reply = manager.post(req, request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(results()));
}

bool App::isError(QString message)
{
    QStringList msgSplit = message.split("|");
    if(msgSplit[0].simplified() == "error"){
        if(msgSplit.length() > 1)
            this->error = msgSplit[1].simplified();
        return true;
    }
    else
        return false;
}

App::~App()
{
    delete reply;
}

void App::results()
{
    QString result = reply->readAll();
    if(GLOBAL::debugging) qDebug() << "result: " + result;
    if(!isError(result)){
        emit finished();
        emit finished(result);
    }
    else{
        if(error == "") error = "Неопознанная ошибка";
        if(error != "hidden")
            MainClass::warning(error);
        emit failed();
    }
    this->deleteLater();
}

