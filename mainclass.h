#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "screenclass.h"
#include "loginclass.h"
#include "global.h"
#include "areaclass.h"
#include "siteopen.h"
#include <windows.h>

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace Ui {
class MainClass;
}

class MainClass : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainClass(QWidget *parent = 0);
    void setTrayIcon();
    void setIconImage(QString icon);
    void createXMLFile();
    void authGui(bool enabled);
    void toAuth();
    QString passHash(QString pass);
    void startAuth();
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    ~MainClass();
private slots:
    void trayActivate(QSystemTrayIcon::ActivationReason r);
    void screen(int x = 0, int y = 0, int w = -1, int h = -1);
    void screenArea();
    void on_quality_valueChanged(int value);
    void uploadProgress(qint64 bytes, qint64 total);
    void uploadFinished();
    void on_signup_clicked();
    void on_login_clicked();
    void authReply(QString rp);
    void on_logout_clicked();
    void openAccountSite();
    void areaGot(int x, int y, int w, int h);

    void on_toAccount_clicked();

private:
    Ui::MainClass *ui;
    QSystemTrayIcon *trayIcon;
    AreaClass *areaScreener;
    QString _email;
    QString _password;
};

#endif // MAINCLASS_H
