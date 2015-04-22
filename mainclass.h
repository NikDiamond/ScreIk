#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "screenclass.h"
#include "loginclass.h"
#include "global.h"
#include "siteopen.h"
#include "keyhooker.h"
#include "hookkeyboard.h"
#include "areascreen.h"
#include "story.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QPixmap>
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
    ~MainClass();
    QString modString(int modId);
    void startSettings();
    void setTrayIcon();
    void setIconImage(QString icon);
    void createXMLFile();
    void authGui(bool enabled);
    void toAuth();
    QString passHash(QString pass);
    void startAuth();
    void storyFill();
    void storyUpdate();
    void setRegRun(bool state);
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private slots:
    void trayActivate(QSystemTrayIcon::ActivationReason r);
    void screen(int x = 0, int y = 0, int w = -1, int h = -1);
    void screenArea();
    void screenWnd();
    void uploadProgress(qint64 bytes, qint64 total);
    void uploadFinished(QString link, QString date);
    void on_signup_clicked();
    void on_login_clicked();
    void authReply(QString rp);
    void on_logout_clicked();
    void openAccountSite();
    void areaGot(int x, int y, int w, int h);
    void areaBroken();
    void openScreen();
    void on_toAccount_clicked();
    void emitPress(HookKeyboard::HookKey key);
    void on_autorunBox_toggled(bool checked);
    void on_jpeg_toggled(bool checked);
    void storyLoaded(QStringList story);
    void on_story1_clicked();
    void on_story2_clicked();
    void on_story3_clicked();

private:
    Ui::MainClass *ui;
    QSystemTrayIcon *trayIcon;
    AreaScreen *areaScreener;
    bool areaBusy;
    QString lastLink;
    QString _email;
    QString _password;
    HookKeyboard *hooker;
    QMap<QString, QStringList> storyList;
};

#endif // MAINCLASS_H
