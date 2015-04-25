#ifndef APP_H
#define APP_H

#include "global.h"
#include "mainclass.h"

#include <QObject>
#include <QDebug>
#include <QMessageBox>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QString act, QString POST, QObject *parent = 0);
    void query();
    bool isError(QString message);
    ~App();
private:
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString act;
    QString POST;
    QString error;
private slots:
    void results();
signals:
    void finished(QString);
    void finished();
    void failed();
};

#endif // APP_H
