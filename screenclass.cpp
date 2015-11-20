#include "screenclass.h"

ScreenClass::ScreenClass(QObject *parent) :
    QObject(parent)
{}
void ScreenClass::doScreen(QString email, QString pass, int x, int y, int w, int h)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    qDebug() << "[" << QTime::currentTime().toString() << "] Screening...";
    if(screen){
        QSettings settings;
        QString ext;
        bool isPNG = settings.value("general/png", false).toBool();
        if(isPNG) ext = "png"; else ext = "jpg";

        srand(QDateTime::currentMSecsSinceEpoch());
        QString fileName = QDir::tempPath()+"\\NikScreen"+QString::number(rand())+"."+ext;
        QPixmap pixmap = screen->grabWindow(0, x, y, w, h);

        QString checkString = fileName;
        QString pngName = checkString.replace(QString(".jpg"), QString(".png"));
        pixmap.save(pngName, "PNG", GLOBAL::quality);

        if(!isPNG){//If not only png, checking jpeg size
            QString jpgName = checkString.replace(QString(".png"), QString(".jpg"));
            pixmap.save(jpgName, "JPG", 100);

            QFileInfo pngInfo(pngName);
            QFileInfo jpgInfo(jpgName);

            if(jpgInfo.size() < pngInfo.size()){
                //saving jpeg, removing png
                fileName = jpgName;
                ext = "jpg";
                QFile *remFile = new QFile(pngName);
                if(remFile->open(QIODevice::ReadOnly))
                    remFile->remove();
                delete remFile;
            }else{
                //saving png, removing jpeg
                fileName = pngName;
                ext = "png";
                QFile *remFile = new QFile(jpgName);
                if(remFile->open(QIODevice::ReadOnly))
                    remFile->remove();
                delete remFile;
            }
        }
        qDebug() << "File: " << fileName << ", q: " << GLOBAL::quality;
        //POSTING FORM DATA
        QUrl url("http://"+GLOBAL::domain+"/upload.php?email="+email+"&passHash="+pass);
        QString bound="margin";
        QByteArray data(QString("--" + bound + "\r\n").toLatin1());
        data.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
        data.append("upload.php\r\n");
        data.append("--" + bound + "\r\n");
        QFileInfo fileInfo(fileName);
        data.append("Content-Disposition: form-data; name=\"file\"; filename=\""+fileInfo.fileName()+"\"\r\n");
        data.append("Content-Type: image/"+ext+"\r\n\r\n");

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
            qDebug() << "[" << QTime::currentTime().toString() << "] File closed";
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
    clipboard->setText("http://"+GLOBAL::domain+"/l/"+rp);

    QDateTime now = QDateTime::currentDateTime();

    emit finished(rp, now.toString("yyyy-MM-dd hh:mm:ss"));
    deleteLater();
}
