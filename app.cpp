#include "app.h"

App::App(QString act, QString POST, QObject *parent): QObject(parent)
{
    this->act = act;
    this->POST = POST;
    this->errorStr = "";
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
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void App::download()
{
    QUrl url(act);
    QNetworkRequest request(url);

    reply = manager.get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(progress(qint64,qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(downloaded()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

bool App::isError(QString message)
{
    QStringList msgSplit = message.split("|");
    if(msgSplit[0].simplified() == "error"){
        if(msgSplit.length() > 1)
            this->errorStr = msgSplit[1].simplified();
        return true;
    }
    else
        return false;
}

App::~App()
{}

void App::results()
{
    QString result = reply->readAll();
    reply->deleteLater();

    if(GLOBAL::debugging) qDebug() << "result: " + result;
    if(!isError(result)){
        emit finished();
        emit finished(result);
    }
    else{
        if(errorStr == "") errorStr = "Неопознанная ошибка";
        if(errorStr != "hidden")
            MainClass::warning(errorStr);
        emit failed();
    }
    this->deleteLater();
}

void App::downloaded()
{
    data = reply->readAll();
    if(GLOBAL::debugging)
        qDebug() << "Download complete | " + act;

    QString path = POST;
    if(path == "") path = QDir::currentPath();
    QFileInfo fileInfo(QUrl(act).path());
    QString fileName = fileInfo.fileName();

    QFile *file = new QFile(path+"\\"+fileName, this);
    file->open(QIODevice::WriteOnly);
    file->write(data);
    file->close();
    if(GLOBAL::debugging)
        qDebug() << "File moved | " + path + "\\" + fileName;

    reply->deleteLater();
    emit finished(path+"\\"+fileName);
    this->deleteLater();
}

void App::progress(qint64 dwn, qint64 total)
{
    emit downloadProgress(dwn, total);
}

void App::error(QNetworkReply::NetworkError error)
{
    emit failed("Ошибка подключения " + error);
    emit failed();
}

