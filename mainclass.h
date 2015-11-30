#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "screenclass.h"
#include "global.h"
#include "keyhooker.h"
#include "hookkeyboard.h"
#include "areascreen.h"
#include "app.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QEventLoop>
#include <QClipboard>
#include <QWidgetAction>
#include <QDebug>
#include <QMenu>
#include <QLabel>
#include <QFileDialog>
#include <QAction>
#include <QSettings>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QProcess>
#include <QCoreApplication>
#include <QByteArray>
#include <QtMultimedia>
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
    void setIconImage(QString icon);
    void createXMLFile();
    QString passHash(QString pass);
    void auth(QString email, QString password);
    void authOnStartUp();
    void storyFill();
    void storyUpdate();
    void updateCheck();
    void updateLoad(QString link);
    void setRegRun(bool state);
    void authGui(bool enabled);
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private slots:
    void trayActivate(QSystemTrayIcon::ActivationReason r);

    void screen(int x = 0, int y = 0, int w = -1, int h = -1);
    void screenArea();
    void screenWnd();

    void uploadProgress(qint64 bytes, qint64 total);
    void uploadFinished(QString link, QString date);
    void uploadFinished(QPixmap pix);
    void authCheck();
    void toAuth();
    void getSecureKey();
    void openSite(QString key);
    void areaGot(int x, int y, int w, int h);
    void areaBroken();
    void openScreen();
    void updateFailed();
    void updateLoaded(QString path);
    void updateProgress(qint64 dwn, qint64 total);
    void emitPress(HookKeyboard::HookKey key);
    void storyLoaded(QString story);
    void updateStart(QString data);
    void finishUpdate();
    void setTrayIcon();
    void setTrayMenu();
    void copyLinkToClp(QString text);
    void deleteScreen(QString name);
    void screenDeleted();
    void openScreen(QString name);

    void on_signup_clicked();
    void on_login_clicked();
    void on_logout_clicked();
    void on_toAccount_clicked();
    void on_autorunBox_toggled(bool checked);
    void on_jpeg_toggled(bool checked);
    void on_story1_clicked();
    void on_story2_clicked();
    void on_story3_clicked();
    void on_update_clicked();
    void on_pathButton_clicked();
    void on_l_1_toggled(bool checked);
    void on_l_2_toggled(bool checked);
    void on_l_3_toggled(bool checked);

public slots:
    static void warning(QString message);
private:
    void uiSetup();
    void getRegistrySettings();

    Ui::MainClass *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    AreaScreen *areaScreener;
    bool areaBusy;
    bool updateChecked;
    int historyLength;
    int saveType;
    QString lastLink;
    QString _email;
    QString _password;
    HookKeyboard *hooker;
    QMap<QString, QStringList> storyList;
    WINBOOL IsElevated();
};

#endif // MAINCLASS_H
