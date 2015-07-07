#ifndef APP_H
#define APP_H

#include "global.h"
#include "mainclass.h"

#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QByteArray>
#include <QFile>
#include <QDir>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QString act, QString POST = "", QObject *parent = 0);
    void query();
    void download();
    bool isError(QString message);
    ~App();
private:
    QNetworkAccessManager manager;
    QByteArray data;
    QNetworkReply *reply;

    QString act;
    QString POST;
    QString errorStr;
private slots:
    void results();
    void downloaded();
    void progress(qint64 dwn, qint64 total);
    void error(QNetworkReply::NetworkError error);
    void error(QString error);
signals:
    void finished(QString);
    void finished();
    void failed(QString);
    void failed();
    void downloadProgress(qint64, qint64);
};

#endif // APP_H
