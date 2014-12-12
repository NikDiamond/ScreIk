#include "mainclass.h"
#include "ui_mainclass.h"


MainClass::MainClass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClass)
{
    ui->setupUi(this);
    startSettings();

    this->setFixedSize(this->size());

    ui->accountGroup->setVisible(false);
    setTrayIcon();
    startAuth();
    hooker = HookKeyboard::instance();
    hooker->startHook();

    connect(hooker, SIGNAL(keyPress(HookKeyboard::HookKey)), this, SLOT(emitPress(HookKeyboard::HookKey)));
    connect(ui->password, SIGNAL(returnPressed()), ui->login, SLOT(click()));
    connect(ui->email, SIGNAL(returnPressed()), ui->login, SLOT(click()));

    GLOBAL::mainId = (HWND)winId();
    ui->keyhook_full->setTarget(0);
    ui->keyhook_area->setTarget(1);
    ui->keyhook_wnd->setTarget(2);
}

void MainClass::emitPress(HookKeyboard::HookKey key)
{
    if(key == static_cast<HookKeyboard::HookKey>(0)){}
    if(QApplication::focusWidget() != NULL){
        QKeyEvent *emitEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Print, Qt::NoModifier, 0, VK_SNAPSHOT, 0, "Prt Screen");
        QKeyEvent *emitEventRelease = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Print, Qt::NoModifier, 0, VK_SNAPSHOT, 0, "Prt Screen");
        QCoreApplication::postEvent(QApplication::focusWidget(), emitEvent);
        QCoreApplication::postEvent(QApplication::focusWidget(), emitEventRelease);
    }
    hooker->endHook();
    keybd_event(VK_SNAPSHOT, MapVirtualKeyA(VK_SNAPSHOT, 0),0,0);
    hooker->startHook();
}

void MainClass::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icon.ico"));
    trayIcon->show();
    QMenu *trayMenu = new QMenu(this);
    QAction *actAcc = new QAction(QIcon("://icons/icon.ico"), "Аккаунт",trayMenu);
    QAction *actScreen = new QAction(QIcon("://icons/fullscreen.ico"), "Скрин",trayMenu);
    QAction *actArea = new QAction(QIcon("://icons/area.ico"), "Скрин области",trayMenu);
    QAction *actWnd = new QAction(QIcon("://icons/window.ico"), "Скрин окна",trayMenu);
    QAction *actOpen = new QAction(QIcon("://icons/settings.ico"), "Настройки",trayMenu);
    QAction *actExit = new QAction(QIcon("://icons/exit.ico"), "Выход",trayMenu);

    trayMenu->addAction(actAcc);
    trayMenu->addSeparator();
    trayMenu->addAction(actScreen);
    trayMenu->addAction(actArea);
    trayMenu->addAction(actWnd);
    trayMenu->addSeparator();
    trayMenu->addAction(actOpen);
    trayMenu->addAction(actExit);
    trayIcon->setContextMenu(trayMenu);

    connect(actAcc, SIGNAL(triggered()), this, SLOT(openAccountSite()));
    connect(actScreen, SIGNAL(triggered()), this, SLOT(screen()));
    connect(actArea, SIGNAL(triggered()), this, SLOT(screenArea()));
    connect(actWnd, SIGNAL(triggered()), this, SLOT(screenWnd()));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(show()));
    connect(actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivate(QSystemTrayIcon::ActivationReason)));
}

void MainClass::setIconImage(QString icon)
{
    trayIcon->setIcon(QIcon(icon));
}

void MainClass::authGui(bool enabled)
{
    ui->login->setEnabled(enabled);
    ui->signup->setEnabled(enabled);
    ui->email->setEnabled(enabled);
    ui->password->setEnabled(enabled);
}

void MainClass::toAuth()
{
    if(!this->isVisible())
        this->show();
    ui->tabWidget->setCurrentIndex(1);
    this->setFocus();
}

QString MainClass::passHash(QString pass)
{
    pass = QString(QCryptographicHash::hash(pass.toLatin1(),QCryptographicHash::Md5).toHex());
    pass = QString(QCryptographicHash::hash(pass.toLatin1(),QCryptographicHash::Md5).toHex());
    return pass;
}
void MainClass::startAuth()
{
    QSettings settings;

    if(settings.value("auth/email", 0) != 0 && settings.value("auth/password", 0) != 0){
        _email = settings.value("auth/email").toString();
        _password = settings.value("auth/password").toString();
        logInClass *login = new logInClass(_email, _password, this);
        login->tryLogin();
        connect(login, SIGNAL(gotReply(QString)), this, SLOT(authReply(QString)));
    }else{
        toAuth();
    }
}

void MainClass::setRegRun(bool state)
{
    if(state){
        #ifdef Q_OS_WIN32
        QSettings autorun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        autorun.setValue("ScreIk",QCoreApplication::applicationFilePath().replace("/","\\"));
        autorun.sync();
        #endif
    }else{
        #ifdef Q_OS_WIN32
        QSettings autorun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        autorun.remove("ScreIk");
        #endif
    }
}

bool MainClass::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == QByteArray() || result == 0){}
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY)
    {
        switch(msg->wParam){
        case 0:
            screen();
        break;
        case 1:
            screenArea();
        break;
        case 2:
            screenWnd();
        break;
        }
    }
    return false;
}
void MainClass::trayActivate(QSystemTrayIcon::ActivationReason r)
{
    if(r==QSystemTrayIcon::Trigger){//click
        if(!this->isVisible()){
            this->show();
            this->activateWindow();
        }
        else
            this->hide();
    }
}

void MainClass::screen(int x, int y, int w, int h)
{
    if(GLOBAL::authorized){
        ScreenClass *screener = new ScreenClass(this);
        screener->doScreen(_email, _password, x, y, w, h);

        connect(screener, SIGNAL(progress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
        connect(screener, SIGNAL(finished()), this, SLOT(uploadFinished()));
    }else{
        toAuth();
    }
}

void MainClass::screenArea()
{
    if(!GLOBAL::authorized){
        if(!this->isVisible())
            this->show();
        ui->tabWidget->setCurrentIndex(1);
        this->setFocus();
    }else{
        //area screen
        areaScreener = new AreaScreen();
        connect(areaScreener, SIGNAL(completed(int,int,int,int)), this, SLOT(areaGot(int,int,int,int)));
    }
}

void MainClass::screenWnd()
{
    if(!GLOBAL::authorized){
        if(!this->isVisible())
            this->show();
        ui->tabWidget->setCurrentIndex(1);
        this->setFocus();
    }else{
        //window screen
        RECT pt;
        GetWindowRect(GetForegroundWindow(),&pt);
        screen(pt.left, pt.top, pt.right-pt.left, pt.bottom-pt.top);
    }
}

void MainClass::areaGot(int x, int y, int w, int h)
{
    delete areaScreener;
    screen(x, y, w, h);
}

MainClass::~MainClass()
{
    delete ui;
    delete trayIcon;
}

QString MainClass::modString(int modId)
{
    switch(modId){
    case MOD_ALT:
        return "Alt + ";
    case MOD_CONTROL:
        return "Ctrl + ";
    case MOD_SHIFT:
        return "Shift + ";
    default:
        return "";
    }
}

void MainClass::startSettings()
{
    QSettings settings;
    if(settings.value("general/autorun", true).toBool())
        setRegRun(true);
    ui->autorunBox->setChecked(settings.value("general/autorun", true).toBool());
    //quality box
    ui->png->setChecked(settings.value("general/png", false).toBool());
    //hotkeys
    RegisterHotKey((HWND)winId(), 0, settings.value("hotkeys/fullscreen_mod", 0).toUInt(), settings.value("hotkeys/fullscreen_key", VK_SNAPSHOT).toUInt());//full
    ui->keyhook_full->setText(modString(settings.value("hotkeys/fullscreen_mod", 0).toUInt()) + settings.value("hotkeys/fullscreen_text", "Print").toString());

    RegisterHotKey((HWND)winId(), 1, settings.value("hotkeys/areascreen_mod", MOD_ALT).toUInt(), settings.value("hotkeys/areascreen_key", VK_SNAPSHOT).toUInt());//area
    ui->keyhook_area->setText(modString(settings.value("hotkeys/areascreen_mod", MOD_ALT).toUInt()) + settings.value("hotkeys/areascreen_text", "Print").toString());

    RegisterHotKey((HWND)winId(), 2, settings.value("hotkeys/wndscreen_mod", MOD_SHIFT).toUInt(), settings.value("hotkeys/wndscreen_key", VK_SNAPSHOT).toUInt());//area
    ui->keyhook_wnd->setText(modString(settings.value("hotkeys/wndscreen_mod", MOD_SHIFT).toUInt()) + settings.value("hotkeys/wndscreen_text", "Print").toString());
}

void MainClass::uploadProgress(qint64 bytes, qint64 total)
{
    if(bytes <= total && bytes > 0){
        int percent = bytes*100/total;
        if(percent <= 20)
            setIconImage(":/icons/icon20.ico");
        else if(percent <= 40)
            setIconImage(":/icons/icon40.ico");
        else if(percent <= 60)
            setIconImage(":/icons/icon60.ico");
        else if(percent <= 80)
            setIconImage(":/icons/icon80.ico");
        else if(percent <= 100)
            setIconImage(":/icons/icon100.ico");
    }
}

void MainClass::uploadFinished()
{
    setIconImage(":/icons/icon.ico");
    qApp->beep();
}

void MainClass::on_signup_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/?p=signup"));
}

void MainClass::on_login_clicked()
{
    ui->errString->clear();
    authGui(false);

    QString email = ui->email->text();
    QString password = ui->password->text();
    password = passHash(password);

    if(email.length() < 4 || email.length() > 254 || password.length() < 6){
        ui->errString->setText("Ошибка ввода.");
        ui->login->setEnabled(true);
        ui->signup->setEnabled(true);
        return;
    }
    logInClass *login = new logInClass(email, password, this);
    login->tryLogin();
    connect(login, SIGNAL(gotReply(QString)), this, SLOT(authReply(QString)));
}

void MainClass::authReply(QString rp)
{
    if(rp == "complete"){
        qDebug() << "Log In successful!";

        GLOBAL::authorized = true;
        ui->accountGroup->setVisible(true);
        ui->authGroup->setVisible(false);

        if(ui->remember->isChecked()){
            _email = ui->email->text();
            _password = passHash(ui->password->text());
            QSettings settings;
            settings.setValue("auth/email", _email);
            settings.setValue("auth/password", _password);
            settings.sync();
        }
        ui->accountEmail->setText(_email);
    }
    else{
        qDebug() << "Log In failed..." << rp;
        toAuth();
    }
    authGui(true);
}

void MainClass::on_logout_clicked()
{
    _email = "";
    _password = "";
    GLOBAL::authorized = false;
    ui->accountGroup->setVisible(false);
    ui->authGroup->setVisible(true);
    ui->accountEmail->clear();
    QSettings settings;
    settings.remove("auth");
    qDebug() << "Logged out!";
}

void MainClass::openAccountSite()
{
    SiteOpen *site = new SiteOpen(_email, _password, this);
    site->use();
}

void MainClass::on_toAccount_clicked()
{
    openAccountSite();
}

void MainClass::on_autorunBox_toggled(bool checked)
{
    if(checked)
        setRegRun(true);
    else
        setRegRun(false);

    QSettings settings;
    settings.setValue("general/autorun", checked);
    settings.sync();
}

void MainClass::on_jpeg_toggled(bool checked)
{
    QSettings settings;
    settings.setValue("general/png", !checked);
    settings.sync();
}
