#ifndef SCREENCLASS_H
#define SCREENCLASS_H

#include "global.h"
#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QByteArray>
#include <QClipboard>
#include <QSettings>
#include <QDateTime>
#include <QTime>
#include <QtOpenGL/QGLWidget>
#include <QtWinExtras/QtWin>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class ScreenClass : public QObject
{
    Q_OBJECT
public:
    explicit ScreenClass(QObject *parent = 0);
    void doScreen(QString email, QString pass, int x = 0, int y = 0, int w = -1, int h = -1);
    ~ScreenClass();
private slots:
    void uploadProgress(qint64 bytes,qint64 total);
    void uploadFinished(QNetworkReply *reply);
private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
signals:
    void progress(qint64, qint64);
    void finished(QString, QString);
};

#endif // SCREENCLASS_H
