#include "screenclass.h"

ScreenClass::ScreenClass(QObject *parent) :
    QObject(parent)
{}
void ScreenClass::doScreen(QString email, QString pass, int x, int y, int w, int h, QString path, int saveType)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QDateTime nw = QDateTime::currentDateTime();
    QString datetime = nw.toString("yyyy-MM-dd hh:mm:ss");

    if(GLOBAL::debugging) qDebug() << "[" << QTime::currentTime().toString() << "] Screening...";
    if(screen){
        QSettings settings;
        QString ext;
        bool isPNG = settings.value("general/png", false).toBool();
        if(isPNG) ext = "png"; else ext = "jpg";

        srand(QDateTime::currentMSecsSinceEpoch());
        QString fileName = QDir::tempPath()+"/ScreIk"+QString::number(rand())+"."+ext;
        QPixmap pixmap = screen->grabWindow(0, x, y, w, h);

        mainPix = pixmap;
        QClipboard *clip = QApplication::clipboard();
        clip->setPixmap(mainPix);

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
        if(GLOBAL::debugging) qDebug() << "File: " << fileName << ", q: " << GLOBAL::quality;

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
            if(GLOBAL::debugging) qDebug() << "[" << QTime::currentTime().toString() << "] File closed";
        }
    }
}
ScreenClass::~ScreenClass()
{
    delete reply;
}
bool ScreenClass::isError(QString message)
{
    if(_status != 200) return true;
    if(message.length() != 16) return true;
    QStringList msgSplit = message.split("|");
    if(msgSplit[0].simplified() == "error")
        return true;
    else
        return false;
}

void ScreenClass::uploadProgress(qint64 bytes, qint64 total)
{
    emit progress(bytes, total);
}

void ScreenClass::uploadFinished(QNetworkReply *reply)
{
    QString rp = reply->readAll();
    if(GLOBAL::debugging) qDebug() << "Screen uploaded. [" << rp << "]";

    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    _status = status.toInt();

    if(!isError(rp)){
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText("http://"+GLOBAL::domain+"/l/"+rp);
        emit finished(rp, date());
    }else{
        emit finished(mainPix);
    }
    deleteLater();
}

QString ScreenClass::date()
{
    QDateTime now = QDateTime::currentDateTime();
    return now.toString("yyyy-MM-dd hh:mm:ss");
}
