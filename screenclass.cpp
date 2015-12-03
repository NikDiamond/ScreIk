#include "screenclass.h"

ScreenClass::ScreenClass(QObject *parent) :
    QObject(parent)
{}
void ScreenClass::doScreen(QString email, QString pass, int x, int y, int w, int h, QString path, int saveType)
{
    _email = email;
    _pass = pass;
    _path = path;
    _saveType = saveType;

    QScreen *screen = QGuiApplication::primaryScreen();
    _mainPix = new QPixmap(screen->grabWindow(0, x, y, w, h));

    if(GLOBAL::useBufferPix){
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(*_mainPix);
    }
    QString localName = QDateTime::currentDateTime().toString("dd-MM-yyyy [hh-mm-ss]");
    if(GLOBAL::debugging) qDebug() << "[" << QTime::currentTime().toString() << "] Screening...";
    //if isPNG is false - choosing between two files
    //otherwise only saving in png
    QSettings settings;
    QString ext;

    bool isPNG = settings.value("general/png", false).toBool();
    if(isPNG) ext = "png"; else ext = "jpg";

    _tempPath = QDir::tempPath()+"/ScreIk"+QString::number(rand())+"."+ext;
    QString pngName = _tempPath.replace(QString(".jpg"), QString(".png"));
    QString jpgName = _tempPath.replace(QString(".png"), QString(".jpg"));
    _mainPix->save(pngName, "PNG", GLOBAL::quality);

    if(!isPNG){//If not only png, checking jpeg size
        _mainPix->save(jpgName, "JPG", 100);

        QFileInfo pngInfo(pngName);
        QFileInfo jpgInfo(jpgName);

        if(jpgInfo.size() < pngInfo.size()){//saving jpeg
            ext = "jpg";
            _tempPath = jpgName;
            QFile::remove(pngName);
        }else{                              //saving png
            ext = "png";
            _tempPath = pngName;
            QFile::remove(jpgName);
        }
    }
    /*
        saveType
        1: yes
        2: if offline
        3: no
        4: only local
        -------------
        | 1,4,2 - save
        | 3 - not save
    */
    if(_saveType != 3){
        QString savePath = _path+"/";
        savePath = savePath.replace("/", "\\");

        QDir dir(savePath);
        if (!dir.exists()) dir.mkpath(".");

        int qual = 100;
        if(ext=="png") qual = GLOBAL::localQuality;

        _localPath = savePath+localName+"."+ext;
        _mainPix->save(_localPath, 0, qual);
        if(GLOBAL::debugging) qDebug () << "[ScreenClass] Screen saved locally";
    }
    //next upload if needed
    if(_saveType==4){//dont uploading
        emit finished(false);
        deleteTemp();

        delete _mainPix;
        delete this;
    }else           //uploading
        uploadScreen(ext);
}

void ScreenClass::uploadScreen(QString ext)
{
    if(GLOBAL::debugging) qDebug() << "[ScreenClass] Uploading...";
    //POSTING FORM DATA
    QUrl url("http://"+GLOBAL::domain+"/upload.php?email="+_email+"&passHash="+_pass);
    QString bound="margin";
    QByteArray data(QString("--" + bound + "\r\n").toLatin1());
    data.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
    data.append("upload.php\r\n");
    data.append("--" + bound + "\r\n");
    QFileInfo fileInfo(_tempPath);
    data.append("Content-Disposition: form-data; name=\"file\"; filename=\""+fileInfo.fileName()+"\"\r\n");
    data.append("Content-Type: image/"+ext+"\r\n\r\n");

    QFile *upFile = new QFile(_tempPath);
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

        upFile->close();
        if(GLOBAL::debugging) qDebug() << "[" << QTime::currentTime().toString() << "] File closed";
    }
    deleteTemp();
}
ScreenClass::~ScreenClass()
{}
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

void ScreenClass::deleteTemp()
{
    QFile::remove(_tempPath);
}

void ScreenClass::uploadProgress(qint64 bytes, qint64 total)
{
    emit progress(bytes, total);
}

void ScreenClass::uploadFinished(QNetworkReply *rep)
{
    QString rp = rep->readAll();
    if(GLOBAL::debugging) qDebug() << "Screen uploaded. [" << rp << "]";

    QVariant status = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    _status = status.toInt();

    if(!isError(rp)){
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText("http://"+GLOBAL::domain+"/l/"+rp);

        //delete saved file if
        //it must savet only if offline
        if(_saveType==2) QFile::remove(_localPath);

        emit finished(rp, date());
    }else{
        emit finished(true);
    }

    delete reply;
    delete _mainPix;
    this->deleteLater();
}

QString ScreenClass::date()
{
    QDateTime now = QDateTime::currentDateTime();
    return now.toString("yyyy-MM-dd hh:mm:ss");
}
