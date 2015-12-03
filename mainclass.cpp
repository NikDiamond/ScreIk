#include "mainclass.h"
#include "ui_mainclass.h"


MainClass::MainClass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClass),
    areaBusy(false)
{
    uiSetup();//Setting up ui
    finishUpdate();
    historyLength = 0;
    authOnStartUp();
    getRegistrySettings();//Setting ui by settings in registry

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

void MainClass::setIconImage(QString icon)
{
    trayIcon->setIcon(QIcon(icon));
}

void MainClass::toAuth()
{
    if(!this->isVisible())
        this->show();
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

void MainClass::uiSetup()
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->accountGroup->setVisible(false);
    //ui->updateBar->setVisible(false);
    ui->aboutLabel->setText("ScreIk. Версия "+GLOBAL::version+" \n\nАвтор: NikDiamond \n\nhttp://nikd.tk/");
    //Set ScreIk logo in "about"
    QPixmap pix(":/img/icons/logo.png");
    ui->logoLB->setPixmap(pix);
    setTrayIcon();
}

void MainClass::getRegistrySettings()
{
    QSettings settings;
    //version
    if(settings.value("version", "").toString() == "" && settings.value("auth/email", "").toString() != ""){
        settings.setValue("version", "0");
        settings.sync();
    }

    QString version = settings.value("version", GLOBAL::version).toString();
    if(version != GLOBAL::version)//just updated
        showUpdateNotes();
    settings.setValue("version", GLOBAL::version);
    settings.sync();
    //
    if(settings.value("general/autorun", true).toBool())
        setRegRun(true);
    ui->autorunBox->setChecked(settings.value("general/autorun", true).toBool());
    //quality box
    ui->png->setChecked(settings.value("general/png", false).toBool());
    //local path
    ui->pathEdit->setText(settings.value("general/path", QCoreApplication::applicationDirPath()).toString());
    saveType = settings.value("general/save", 2).toInt();
    switch(saveType){
    case 1:
        ui->l_1->setChecked(true);
        break;
    case 2:
        ui->l_2->setChecked(true);
        break;
    case 3:
        ui->l_3->setChecked(true);
        break;
    case 4:
        ui->l_4->setChecked(true);
        break;
    }
    //local quality
    ui->locQual_slid->setValue(settings.value("general/local_quality", GLOBAL::quality).toInt());
    GLOBAL::localQuality = settings.value("general/local_quality", GLOBAL::quality).toInt();
    //hotkeys
    RegisterHotKey((HWND)winId(), 0, settings.value("hotkeys/fullscreen_mod", 0).toUInt(), settings.value("hotkeys/fullscreen_key", VK_SNAPSHOT).toUInt());//full
    ui->keyhook_full->setText(modString(settings.value("hotkeys/fullscreen_mod", 0).toUInt()) + settings.value("hotkeys/fullscreen_text", "Print").toString());

    RegisterHotKey((HWND)winId(), 1, settings.value("hotkeys/areascreen_mod", MOD_ALT).toUInt(), settings.value("hotkeys/areascreen_key", VK_SNAPSHOT).toUInt());//area
    ui->keyhook_area->setText(modString(settings.value("hotkeys/areascreen_mod", MOD_ALT).toUInt()) + settings.value("hotkeys/areascreen_text", "Print").toString());

    RegisterHotKey((HWND)winId(), 2, settings.value("hotkeys/wndscreen_mod", MOD_SHIFT).toUInt(), settings.value("hotkeys/wndscreen_key", VK_SNAPSHOT).toUInt());//window
    ui->keyhook_wnd->setText(modString(settings.value("hotkeys/wndscreen_mod", MOD_SHIFT).toUInt()) + settings.value("hotkeys/wndscreen_text", "Print").toString());
}

void MainClass::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icon.ico"));
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivate(QSystemTrayIcon::ActivationReason)));
    setTrayMenu();
}

void MainClass::setTrayMenu()
{
    trayMenu = new QMenu(this);
    QAction *actAcc = new QAction(QIcon("://icons/icon.ico"), "Открыть скриншоты",trayMenu);
    QAction *actScreen = new QAction(QIcon("://icons/fullscreen.ico"), "Скрин",trayMenu);
    QAction *actArea = new QAction(QIcon("://icons/area.ico"), "Скрин области",trayMenu);
    QAction *actOpen = new QAction(QIcon("://icons/settings.ico"), "Настройки",trayMenu);
    QAction *actExit = new QAction(QIcon("://icons/exit.ico"), "Выход",trayMenu);

    trayMenu->addAction(actAcc);
    trayMenu->addSeparator();
    //add screens
    //copy link mapper
    QSignalMapper * linkMapper = new QSignalMapper(this);
    QObject::connect(linkMapper,SIGNAL(mapped(QString)),this,SLOT(copyLinkToClp(QString)));
    //delete mapper
    QSignalMapper * delMapper = new QSignalMapper(this);
    QObject::connect(delMapper,SIGNAL(mapped(QString)),this,SLOT(deleteScreen(QString)));
    //open mapper
    QSignalMapper * openMapper = new QSignalMapper(this);
    QObject::connect(openMapper,SIGNAL(mapped(QString)),this,SLOT(openScreen(QString)));
    if(!storyList.isEmpty()){
        for(int i = 0; i < historyLength; i++){
            QAction *action = new QAction(storyList["dates"][i], trayMenu);
            trayMenu->addAction(action);
            QMenu *screenMenu = new QMenu(this);
            //submenu
            //image
            QAction *labelAct = new QAction(screenMenu);
            labelAct->setIcon(QIcon("://icons/fullscreen.ico"));
            screenMenu->addAction(labelAct);
            //open
            QAction *open = new QAction(QIcon("://icons/exit.ico"), "Открыть в браузере", this);
            screenMenu->addAction(open);
            openMapper->setMapping(open, storyList["names"][i]);
            connect(open, SIGNAL(triggered()), openMapper, SLOT(map()));
            //copy link
            QAction *link = new QAction("Копировать ссылку в буфер обмена", this);
            screenMenu->addAction(link);
            linkMapper->setMapping(link, storyList["names"][i]);
            connect(link, SIGNAL(triggered()), linkMapper, SLOT(map()));
            //
            screenMenu->addSeparator();
            //delete
            QAction *del = new QAction("Удалить", this);
            screenMenu->addAction(del);
            delMapper->setMapping(del, storyList["names"][i]);
            connect(del, SIGNAL(triggered()), delMapper, SLOT(map()));

            action->setMenu(screenMenu);
        }
    }
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
}

void MainClass::copyLinkToClp(QString text)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText("http://"+GLOBAL::domain+"/l/"+text);
}

void MainClass::deleteScreen(QString name)
{
    if(GLOBAL::debugging) qDebug() << "deleting "+name;
    App *app = new App("deleteScreen", "email="+_email+"&password="+_password+"&name="+name.simplified());
    app->query();
    connect(app, SIGNAL(finished()), this, SLOT(screenDeleted()));
}

void MainClass::screenDeleted()
{
    if(GLOBAL::debugging) qDebug() << "deleted";
    storyFill();
}

void MainClass::openScreen(QString name)
{
    QDesktopServices::openUrl(QUrl("http://"+GLOBAL::domain+"/l/"+name));
}

void MainClass::auth(QString email, QString password)
{
    if(GLOBAL::debugging) qDebug() << "Login with email "+email+" and password "+password;
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

    if(ui->email->text() != "" && _email=="" && _password==""){
        _email = ui->email->text();
        _password = passHash(ui->password->text());
        QSettings settings;
        settings.setValue("auth/email", _email);
        settings.setValue("auth/password", _password);
        settings.sync();
    }
    ui->accountEmail->setText(_email);
    ui->loginBar->setMaximum(1);
    authGui(true);
    //next requests
    storyFill();
}

void MainClass::authOnStartUp()
{
    QSettings settings;

    if(settings.value("auth/email", 0) != 0 && settings.value("auth/password", 0) != 0){
        _email = settings.value("auth/email").toString();
        _password = settings.value("auth/password").toString();

        ui->email->setText(_email);

        auth(_email, _password);

    }else{
        toAuth();
    }
}

void MainClass::storyFill()
{
    App *app = new App("getStory", "email="+_email+"&password="+_password+"&length="+historyLength);
    app->query();
    connect(app, SIGNAL(finished(QString)), this, SLOT(storyLoaded(QString)));
}

void MainClass::storyLoaded(QString story)
{
    historyLength = 0;
    story = story.simplified();
    if(story != "empty"){
        QStringList screens = story.split('|');
        storyList.clear();
        for(int i = 0; i < screens.length(); i++){
            storyList["names"].insert(i, screens[i].split('/')[0]);
            storyList["dates"].insert(i, screens[i].split('/')[1]);
            historyLength++;
        }
        storyUpdate();//updating story list
    }
    if(!updateChecked) updateCheck();//checking for updates
}

void MainClass::storyUpdate()
{
    QList<QPushButton*> buttons;
    buttons << ui->story1 << ui->story2 << ui->story3;
    for(int i = 0; i < storyList["names"].length(); i++){
        buttons[i]->setText(storyList["dates"][i]);
        buttons[i]->setEnabled(true);
    }
    trayMenu->clear();
    setTrayMenu();
}

void MainClass::updateCheck()
{
    updateChecked = true;
    ui->update->setText("Проверка...");
    ui->update->setEnabled(false);

    ui->updateBar->setMinimum(0);
    ui->updateBar->setMaximum(0);
    ui->updateBar->setVisible(true);

    App *app = new App("update");
    app->query();
    connect(app, SIGNAL(finished(QString)), this, SLOT(updateStart(QString)));
    connect(app, SIGNAL(failed()), this, SLOT(updateFailed()));
}

BOOL MainClass::IsElevated( ) {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess( ),TOKEN_QUERY,&hToken ) ) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if( hToken ) {
        CloseHandle( hToken );
    }
    return fRet;
}

void MainClass::updateStart(QString data)
{
    QStringList updateData = data.split("|");
    QString version = updateData[0];
    QString link = updateData[1];
    if(version != GLOBAL::version){//Updating
        if(IsElevated()){
            updateLoad(link);
        }else{
            //asking for update
            QMessageBox askBox;
            askBox.setStyleSheet("QMessageBox{background-image: url(:/img/icons/background.png);color: #ffffff;}QWidget{\n	color: #fff;\n	font: \"Roboto\";\n}\nQMenu{\n	color: #000;\n}\nQRadioButton:indicator{\n	background-image: url(:/img/icons/radio.png);\n	width: 20px;\n	height: 20px;\n}\nQRadioButton:indicator:hover{\n	background-image: url(:/img/icons/radio_hover.png);\n}\nQRadioButton:indicator:pressed{\n	background-image: url(:/img/icons/radio_pressed.png);\n}\nQRadioButton:indicator:checked{\n	background-image: url(:/img/icons/radio_checked.png);\n}\nQCheckBox:indicator{\n	background-image: url(:/img/icons/check.png);\n	width: 15px;\n	height: 15px;\n}\nQCheckBox:indicator:hover{\n	background-image: url(:/img/icons/check_hover.png);\n}\nQCheckBox:indicator:pressed{\n	background-image: url(:/img/icons/check_pressed.png);\n}\nQCheckBox:indicator:checked{\n	background-image: url(:/img/icons/check_checked.png);\n}\nQPushButton{\n	background: transparent;\n	border: 2px solid #fff;\n	padding: 4px;\n}\nQPushButton:hover{\n	background-image: url(:/img/icons/tpbg.png);\n}\nQLineEdit{\n	background: transparent;\n	border: 2px solid #fff;\n	padding: 2px;\n}");
            askBox.setText("Доступно обновление.\nТекущая версия: "+GLOBAL::version+"\nНовая версия: "+version+"\n\nОбновить?");
            askBox.setWindowTitle("Обновление");
            askBox.setStandardButtons(QMessageBox::Yes);
            askBox.addButton(QMessageBox::No);
            askBox.setDefaultButton(QMessageBox::Yes);

            if (askBox.exec() == QMessageBox::Yes){
                //Reloading app with admin rights to update if needs
                QString fileP = QCoreApplication::applicationFilePath();
                QString doing = "runas";

                int res = (int)ShellExecute(NULL, reinterpret_cast<const WCHAR*>(doing.utf16()), reinterpret_cast<const WCHAR*>(fileP.utf16()), NULL, NULL, SW_SHOWNORMAL);
                qDebug() << res;
                qApp->quit();
            }else{
                ui->update->setText("Обновить");
                ui->update->setEnabled(true);
                ui->updateBar->setVisible(false);
                //Ставим флаг "не обновлять"
            }
        }
    }else{
        ui->update->setText("Обновить");
        ui->update->setEnabled(true);
        ui->updateBar->setVisible(false);
    }
}

void MainClass::finishUpdate()
{
    QString oldFile = QDir::currentPath()+"\\ScreIk.exe.old";
    if(QFile::exists(oldFile)){
        QFile::remove(oldFile);
        if(GLOBAL::debugging) qDebug() << "Old file "+oldFile+" was removed";
    }
}

void MainClass::updateLoad(QString link)
{
    ui->update->setText("Обновление...");

    App *app = new App(link, QCoreApplication::applicationDirPath());
    app->download();
    connect(app, SIGNAL(finished(QString)), this, SLOT(updateLoaded(QString)));
    connect(app, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgress(qint64,qint64)));
    connect(app, SIGNAL(failed()), this, SLOT(updateFailed()));
}

void MainClass::updateProgress(qint64 dwn, qint64 total)
{
    ui->updateBar->setVisible(true);
    ui->updateBar->setMinimum(0);
    ui->updateBar->setMaximum(total);
    ui->updateBar->setValue(dwn);
}

void MainClass::updateFailed()
{
    ui->update->setText("Обновить");
    ui->update->setEnabled(true);
    ui->updateBar->setVisible(false);

    warning("Ошибка загрузки обновления");
    ui->updateBar->setVisible(false);
    ui->update->setText("Обновить");
    ui->update->setEnabled(true);
}

void MainClass::updateLoaded(QString path)
{
    ui->updateBar->setVisible(false);
    ui->update->setText("Обновить");
    ui->update->setEnabled(true);
    QFileInfo info(path);
    if(QFile::exists(info.path()+"\\ScreIkUpdate.sc")) QFile::remove(info.path()+"\\ScreIkUpdate.sc");
    if(QFile::rename(info.absoluteFilePath(), info.path()+"\\ScreIkUpdate.sc")){
        //Now here is 2 files: screik.exe and screikupdate.sc
        //we must close screik.exe, then delete it and renamescreikupdate.exe to screik.exe
        //by console or smth else
        QString appPath = QCoreApplication::applicationFilePath();
            appPath.replace("/", "\\");
        QString updatePath = info.path()+"\\ScreIkUpdate.sc";
            updatePath.replace("/", "\\");

        QFile::rename(appPath, info.path()+"\\ScreIk.exe.old");
        if(QFile::rename(updatePath, info.path()+"\\ScreIk.exe")){
            QProcess *updated = new QProcess(this);
            updated->startDetached(info.path()+"\\ScreIk.exe", QStringList() << "");
            qApp->quit();
        }
    }
}

void MainClass::setRegRun(bool state)
{
    #ifdef Q_OS_WIN32
    if(state){
        QSettings autorun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        autorun.setValue("ScreIk",QCoreApplication::applicationFilePath().replace("/","\\"));
        autorun.sync();
    }else{
        QSettings autorun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        autorun.remove("ScreIk");
    }
    #endif
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
    setIconImage(":/icons/icon-wait.ico");

    ScreenClass *screener = new ScreenClass(this);

    connect(screener, SIGNAL(progress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    connect(screener, SIGNAL(finished(bool)), this, SLOT(uploadFinished(bool)));
    connect(screener, SIGNAL(finished(QString, QString)), this, SLOT(uploadFinished(QString, QString)));

    screener->doScreen(_email, _password, x, y, w, h, ui->pathEdit->text(), saveType);
}

void MainClass::uploadFinished(bool alert)
{
    doneSound();
    if(!alert) return;

    //show info
    QString mes;
    if(!GLOBAL::authorized)
        mes = "Вы не авторизованы.";
    else
        mes = "Ошибка подключения к серверу.";

    mes += "\nСкриншот сохранён в буфер обмена";
    if(saveType == 2) mes += " и на диск."; else mes+=".";
    trayIcon->showMessage("ScreIk", mes, QSystemTrayIcon::Information, 600);
}

void MainClass::uploadFinished(QString link, QString date)//finished uploading screen
{
    lastLink = link;
    doneSound();

    //updating storyList
    if(storyList["names"].length()<3){
        storyList["names"].insert(storyList["names"].length(), "");
        storyList["dates"].insert(storyList["names"].length(), "");
        historyLength++;
    }
    for(int i = storyList["names"].length()-1; i>0; i--){
        storyList["names"][i] = storyList["names"][i-1];
        storyList["dates"][i] = storyList["dates"][i-1];
    }
    storyList["names"][0] = link;
    storyList["dates"][0] = date;
    storyUpdate();
}

void MainClass::screenArea()
{
    if(!areaBusy){
        if(GLOBAL::debugging) qDebug() << "area";
        areaBusy = true;
        areaScreener = new AreaScreen();
        connect(areaScreener, SIGNAL(completed(int,int,int,int)), this, SLOT(areaGot(int,int,int,int)));
        connect(areaScreener, SIGNAL(broken()), this, SLOT(areaBroken()));
    }
}

void MainClass::screenWnd()
{
    //window screen
    RECT pt;
    GetWindowRect(GetForegroundWindow(),&pt);
    screen(pt.left, pt.top, pt.right-pt.left, pt.bottom-pt.top);
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
        else if(percent < 100)
            setIconImage(":/icons/icon100.ico");
        else if(percent == 100)
            setIconImage(":/icons/icon-wait.ico");
    }
}


void MainClass::doneSound()
{
    setIconImage(":/icons/icon.ico");
    QSound::play(":/snd/sound/complete.wav");
}

void MainClass::showUpdateNotes()
{
    App *app = new App("updateNotes", "version="+GLOBAL::version);
    app->query();
    connect(app, SIGNAL(finished(QString)), this, SLOT(notesShow(QString)));
}

void MainClass::notesShow(QString notes)
{
    QStringList nts = notes.split("|");
    if(notes.simplified() != "")
        warning("Обновление от "+nts[1]+" :\n\n"+nts[0]);
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
        authGui(true);
        return;
    }
    ui->loginBar->setMaximum(0);
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
    if(GLOBAL::debugging) qDebug() << "Logged out!";
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

void MainClass::on_update_clicked()
{
    updateCheck();
}

void MainClass::on_pathButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    QString directory;
    directory = dialog.getExistingDirectory(this, "Выберите путь");
    if(directory.simplified() != ""){
        QFileInfo fi(directory);

        if(fi.isDir() && fi.isWritable()){
            ui->pathEdit->setText(directory);
        }
    }

    QSettings settings;
    settings.setValue("general/path", directory);
    settings.sync();
}

void MainClass::on_l_1_toggled(bool checked)//Всегда
{
    locQualEnabled(true);

    if(!checked) return;
    QSettings settings;
    settings.setValue("general/save", 1);
    settings.sync();
    saveType = 1;
}

void MainClass::on_l_2_toggled(bool checked)//Когда оффлайн
{
    locQualEnabled(true);

    if(!checked) return;
    QSettings settings;
    settings.setValue("general/save", 2);
    settings.sync();
    saveType = 2;
}

void MainClass::on_l_3_toggled(bool checked)//Никогда
{
    locQualEnabled(false);

    if(!checked) return;
    QSettings settings;
    settings.setValue("general/save", 3);
    settings.sync();
    saveType = 3;
}

void MainClass::on_l_4_toggled(bool checked)
{
    locQualEnabled(true);

    if(!checked) return;
    QSettings settings;
    settings.setValue("general/save", 4);
    settings.sync();
    saveType = 4;
}

void MainClass::locQualEnabled(bool state)
{
    ui->locQual_lab->setVisible(state);
    ui->locQual_slid->setVisible(state);
    ui->locQual_res->setVisible(state);
}

void MainClass::on_locQual_slid_sliderMoved(int position)
{
    GLOBAL::localQuality = position;
    QSettings settings;
    settings.setValue("general/local_quality", position);
    settings.sync();
}

void MainClass::on_locQual_res_clicked()
{
    GLOBAL::localQuality = GLOBAL::quality;
    ui->locQual_slid->setValue(GLOBAL::quality);
    QSettings settings;
    settings.remove("general/local_quality");
}
