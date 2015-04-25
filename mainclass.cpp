#include "mainclass.h"
#include "ui_mainclass.h"


MainClass::MainClass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClass)
{
    ui->setupUi(this);
    areaBusy = false;
    startSettings();

    QPixmap pix(":/img/icons/logo.png");
    ui->logoLB->setPixmap(pix);

    this->setFixedSize(this->size());

    ui->accountGroup->setVisible(false);
    setTrayIcon();
    authOnStartUp();
    UpdateCheck();
    if(GLOBAL::authorized)
        storyFill();
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
    QAction *actOpen = new QAction(QIcon("://icons/settings.ico"), "Настройки",trayMenu);
    QAction *actExit = new QAction(QIcon("://icons/exit.ico"), "Выход",trayMenu);

    trayMenu->addAction(actAcc);
    trayMenu->addSeparator();
    trayMenu->addAction(actScreen);
    trayMenu->addAction(actArea);
    trayMenu->addSeparator();
    trayMenu->addAction(actOpen);
    trayMenu->addAction(actExit);
    trayIcon->setContextMenu(trayMenu);

    connect(actAcc, SIGNAL(triggered()), this, SLOT(getSecureKey()));
    connect(actScreen, SIGNAL(triggered()), this, SLOT(screen()));
    connect(actArea, SIGNAL(triggered()), this, SLOT(screenArea()));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(show()));
    connect(actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivate(QSystemTrayIcon::ActivationReason)));
}

void MainClass::setIconImage(QString icon)
{
    trayIcon->setIcon(QIcon(icon));
}

void MainClass::toAuth()
{
    if(!this->isVisible())
        this->show();
    ui->tabWidget->setCurrentIndex(1);
    this->setFocus();
    authGui(true);
}

QString MainClass::passHash(QString pass)
{
    pass = QString(QCryptographicHash::hash(pass.toLatin1(),QCryptographicHash::Md5).toHex());
    pass = QString(QCryptographicHash::hash(pass.toLatin1(),QCryptographicHash::Md5).toHex());
    return pass;
}

void MainClass::authGui(bool enabled)
{
    ui->login->setEnabled(enabled);
    ui->signup->setEnabled(enabled);
    ui->email->setEnabled(enabled);
    ui->password->setEnabled(enabled);
}

void MainClass::warning(QString message)
{
    QMessageBox errorBox;
    errorBox.setStyleSheet("QMessageBox{background-image: url(:/img/icons/background.png);color: #ffffff;}QWidget{\n	color: #fff;\n	font: \"Roboto\";\n}\nQMenu{\n	color: #000;\n}\nQRadioButton:indicator{\n	background-image: url(:/img/icons/radio.png);\n	width: 20px;\n	height: 20px;\n}\nQRadioButton:indicator:hover{\n	background-image: url(:/img/icons/radio_hover.png);\n}\nQRadioButton:indicator:pressed{\n	background-image: url(:/img/icons/radio_pressed.png);\n}\nQRadioButton:indicator:checked{\n	background-image: url(:/img/icons/radio_checked.png);\n}\nQCheckBox:indicator{\n	background-image: url(:/img/icons/check.png);\n	width: 15px;\n	height: 15px;\n}\nQCheckBox:indicator:hover{\n	background-image: url(:/img/icons/check_hover.png);\n}\nQCheckBox:indicator:pressed{\n	background-image: url(:/img/icons/check_pressed.png);\n}\nQCheckBox:indicator:checked{\n	background-image: url(:/img/icons/check_checked.png);\n}\nQPushButton{\n	background: transparent;\n	border: 2px solid #fff;\n	padding: 4px;\n}\nQPushButton:hover{\n	background-image: url(:/img/icons/tpbg.png);\n}\nQLineEdit{\n	background: transparent;\n	border: 2px solid #fff;\n	padding: 2px;\n}");
    errorBox.setText(message);
    QPixmap warning(":/img/icons/warning.png");
    errorBox.setIconPixmap(warning);
    errorBox.exec();
}

void MainClass::auth(QString email, QString password)
{
    App *app = new App("login", "email="+email+"&password="+password);
    app->query();
    connect(app, SIGNAL(finished()), this, SLOT(authCheck()));
    connect(app, SIGNAL(failed()), this, SLOT(toAuth()));
}

void MainClass::authCheck()
{
    GLOBAL::authorized = true;
    ui->accountGroup->setVisible(true);
    ui->authGroup->setVisible(false);

    if(ui->email->text() != ""){
        _email = ui->email->text();
        _password = passHash(ui->password->text());
        QSettings settings;
        settings.setValue("auth/email", _email);
        settings.setValue("auth/password", _password);
        settings.sync();
    }
    ui->accountEmail->setText(_email);
    authGui(true);
    storyFill();
}

void MainClass::authOnStartUp()
{
    QSettings settings;

    if(settings.value("auth/email", 0) != 0 && settings.value("auth/password", 0) != 0){
        _email = settings.value("auth/email").toString();
        _password = settings.value("auth/password").toString();

        auth(_email, _password);
    }else{
        toAuth();
    }
}

void MainClass::storyFill()
{
    int length = 3;
    App *app = new App("getStory", "email="+_email+"&password="+_password+"&length="+length);
    app->query();
    connect(app, SIGNAL(finished(QString)), this, SLOT(storyLoaded(QString)));
}

void MainClass::storyLoaded(QString story)
{
    story = story.simplified();
    QStringList screens = story.split('|');
    for(int i = 0; i < screens.length(); i++){
        storyList["names"].insert(i, screens[i].split('/')[0]);
        storyList["dates"].insert(i, screens[i].split('/')[1]);
    }
    storyUpdate();
}

void MainClass::storyUpdate()
{
    QList<QPushButton*> buttons;
    buttons << ui->story1 << ui->story2 << ui->story3;
    for(int i = 0; i < storyList["names"].length(); i++){
        buttons[i]->setText(storyList["dates"][i]);
        buttons[i]->setEnabled(true);
    }
}

void MainClass::UpdateCheck()
{

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
        connect(screener, SIGNAL(finished(QString, QString)), this, SLOT(uploadFinished(QString, QString)));
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
        if(!areaBusy){
            qDebug() << "area";
            areaBusy = true;
            areaScreener = new AreaScreen();
            connect(areaScreener, SIGNAL(completed(int,int,int,int)), this, SLOT(areaGot(int,int,int,int)));
            connect(areaScreener, SIGNAL(broken()), this, SLOT(areaBroken()));
        }
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
    areaBusy = false;
    screen(x, y, w, h);
}

void MainClass::areaBroken()
{
    areaBusy = false;
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

void MainClass::uploadFinished(QString link, QString date)
{
    lastLink = link;
    setIconImage(":/icons/icon.ico");
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(openScreen()));
    QSound::play(":/snd/sound/complete.wav");

    //updating storyList
    for(int i = storyList["names"].length()-1; i>0; i--){
        storyList["names"][i] = storyList["names"][i-1];
        storyList["dates"][i] = storyList["dates"][i-1];
    }
    storyList["names"][0] = link;
    storyList["dates"][0] = date;
    storyUpdate();
}

void MainClass::openScreen()
{
    trayIcon->disconnect(this, SLOT(openScreen()));
    QDesktopServices::openUrl(QUrl(lastLink));
}

void MainClass::on_signup_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/?p=signup"));
}

void MainClass::on_login_clicked()
{
    QString email = ui->email->text();
    QString password = ui->password->text();
    password = passHash(password);

    if(email.length() < 4 || email.length() > 254 || password.length() < 6){
        ui->login->setEnabled(true);
        ui->signup->setEnabled(true);
        return;
    }
    authGui(false);
    auth(email, password);
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

void MainClass::getSecureKey()
{
    App *app = new App("getAuthKey", "email=" + _email + "&password=" + _password);
    app->query();
    connect(app, SIGNAL(finished(QString)), this, SLOT(openSite(QString)));
}

void MainClass::openSite(QString key)
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/?p=login&key="+key));
}

void MainClass::on_toAccount_clicked()
{
    getSecureKey();
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

void MainClass::on_story1_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/l/"+storyList["names"][0]));
}
void MainClass::on_story2_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/l/"+storyList["names"][1]));
}
void MainClass::on_story3_clicked()
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/l/"+storyList["names"][2]));
}
