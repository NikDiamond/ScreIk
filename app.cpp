#include "app.h"

App::App(QString act, QString POST, bool showErrors, QObject *parent): QObject(parent)
{
    this->act = act;
    this->POST = POST;
    this->errorStr = "";

    this->_status = 200;
    this->retryCount = 0;
    this->showErrors = showErrors;
    this->delaySecs = 2;
}

void App::query()
{
    QUrl url("http://"+GLOBAL::domain+"/app.php?a="+act);
    QByteArray request(POST.toLocal8Bit());
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, GLOBAL::userAgent);

    if(GLOBAL::debugging) qDebug() << "request to server: " + act;
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
    if(_status != 200) return true;
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

void App::retry()
{
    retryCount++;
    if(retryCount>3){
        if(showErrors && errorStr != "hidden") MainClass::warning(errorStr+"\nПри выполнении: "+act);
        error(errorStr);
        return;
    }

    delay(delaySecs);
    if(GLOBAL::debugging) qDebug() << ""; qDebug() << "retrying ("+QString::number(retryCount)+") ...";
    _status = 200;
    query();
}

void App::delay(int secs)
{
    QTime dieTime= QTime::currentTime().addSecs(secs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void App::results()
{
    QString result = reply->readAll();
    //
    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int stat = status.toInt();
    _status = stat;

    if(GLOBAL::debugging) qDebug() << "STATUS: " + status.toString();
    //
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        if(GLOBAL::debugging) qDebug() << reply->errorString();
        errorStr = reply->errorString();
        retry();
        return;
    }

    if(GLOBAL::debugging) qDebug() << "result: " + result;
    if(!isError(result)){
        emit finished();
        emit finished(result);
        this->deleteLater();
    }
    else{
        if(errorStr == "") errorStr = "Ошибка сервера (STATUS: "+QString::number(_status)+")";
        retry();
        return;
    }
}

void App::downloaded()
{
    data = reply->readAll();
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        if(GLOBAL::debugging) qDebug() << reply->errorString();
        emit error(reply->errorString());
        this->deleteLater();
        return;
    }

    if(GLOBAL::debugging) qDebug() << "Download complete | " + act;

    QString path = POST;
    if(path == "") path = QDir::currentPath();
    QFileInfo fileInfo(QUrl(act).path());
    QString fileName = fileInfo.fileName();

    QFile *file = new QFile(path+"\\"+fileName, this);
    file->open(QIODevice::WriteOnly);
    file->write(data);
    file->close();
    if(GLOBAL::debugging) qDebug() << "File moved | " + path + "\\" + fileName;

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
}
void App::error(QString error)
{
    emit failed("Ошибка " + error);
}

