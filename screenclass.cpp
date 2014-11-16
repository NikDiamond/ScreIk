#include "screenclass.h"

ScreenClass::ScreenClass(QObject *parent) :
    QObject(parent)
{}

void ScreenClass::doScreen(QString email, QString pass, int x, int y, int w, int h)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen){
        srand(QDateTime::currentMSecsSinceEpoch());
        QString fileName = QDir::tempPath()+"\\NikScreen"+QString::number(rand())+".png";
        qDebug() << "File: " << fileName << ", q: " << GLOBAL::quality;

        QPixmap pixmap = screen->grabWindow(0, x, y, w, h);
        pixmap.save(fileName, "PNG", GLOBAL::quality);

        //POSTING FORM DATA
        QUrl url("http://"+GLOBAL::domain+"/upload.php?email="+email+"&passHash="+pass);
        QString bound="margin";
        QByteArray data(QString("--" + bound + "\r\n").toLatin1());
        data.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
        data.append("upload.php\r\n");
        data.append("--" + bound + "\r\n");
        QFileInfo fileInfo(fileName);
        data.append("Content-Disposition: form-data; name=\"file\"; filename=\""+fileInfo.fileName()+"\"\r\n");
        data.append("Content-Type: image/png\r\n\r\n");

        QFile *upFile = new QFile(fileName);
        if(upFile->open(QIODevice::ReadOnly)){
            data.append(upFile->readAll());
            data.append("\r\n");
            data.append("--" + bound + "--\r\n");

            QNetworkRequest request(url);

            request.setRawHeader(QString("Content-Type").toLatin1(),QString("multipart/form-data; boundary=" + bound).toLatin1());
            request.setRawHeader(QString("Content-Length").toLatin1(), QString::number(data.length()).toLatin1());

            reply = manager.post(request,data);
            connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(uploadFinished(QNetworkReply*)));
            connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(uploadProgress(qint64,qint64)));
            upFile->remove();
            upFile->close();
        }
    }
}
ScreenClass::~ScreenClass()
{
    delete reply;
}

void ScreenClass::uploadProgress(qint64 bytes, qint64 total)
{
    emit progress(bytes, total);
}

void ScreenClass::uploadFinished(QNetworkReply *reply)
{
    QString rp = reply->readAll();
    qDebug() << "Screen uploaded. [" << rp << "]";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(rp);

    emit finished();
    deleteLater();
}
