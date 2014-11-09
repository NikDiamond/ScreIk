#include "mainclass.h"
#include "ui_mainclass.h"

MainClass::MainClass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClass)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->quality->setValue(GLOBAL::quality);
    ui->accountGroup->setVisible(false);
    setTrayIcon();
    startAuth();

    connect(ui->password, SIGNAL(returnPressed()), ui->login, SLOT(click()));
    connect(ui->email, SIGNAL(returnPressed()), ui->login, SLOT(click()));
}

void MainClass::setTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icon.ico"));
    trayIcon->show();
    QMenu *trayMenu = new QMenu(this);
    QAction *actOpen = new QAction("Открыть",trayMenu);
    QAction *actScreen = new QAction("Скрин",trayMenu);
    QAction *actArea = new QAction("Скрин области",trayMenu);
    QAction *actExit = new QAction("Выход",trayMenu);

    trayMenu->addAction(actOpen);
    trayMenu->addAction(actScreen);
    trayMenu->addAction(actArea);
    trayMenu->addAction(actExit);
    trayIcon->setContextMenu(trayMenu);

    connect(actOpen, SIGNAL(triggered()), this, SLOT(show()));
    connect(actScreen, SIGNAL(triggered()), this, SLOT(screen()));
    connect(actArea, SIGNAL(triggered()), this, SLOT(screenArea()));
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
    QFile configFile(QCoreApplication::applicationDirPath() + "\\auth.xml");
    if(configFile.exists()){
        if(configFile.open(QIODevice::ReadOnly)){
            QXmlStreamReader xmlReader;
            xmlReader.setDevice(&configFile);

            QString email;
            QString password;
            xmlReader.readNext();
            while(!xmlReader.isEndDocument()){
                if(xmlReader.isStartElement()){
                    if(xmlReader.name() == "email"){
                        email = xmlReader.readElementText();
                        _email = email;
                    }
                    if(xmlReader.name() == "password"){
                        password = xmlReader.readElementText();
                        _password = password;
                        break;
                    }
                }
                xmlReader.readNext();
            }
            logInClass *login = new logInClass(email, password, this);
            login->tryLogin();
            connect(login, SIGNAL(gotReply(QString)), this, SLOT(authReply(QString)));
        }else{
            toAuth();
            qDebug() << "Config open error!";
        }
    }else{
        toAuth();
    }
}
void MainClass::trayActivate(QSystemTrayIcon::ActivationReason r)
{
    if(r==QSystemTrayIcon::Trigger){//click
        toAuth();
    }else if(r==QSystemTrayIcon::MiddleClick){//debug
        qDebug() << QApplication::topLevelWidgets();
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
        areaScreener = new AreaClass();
        connect(areaScreener, SIGNAL(completed(int,int,int,int)), this, SLOT(areaGot(int,int,int,int)));
    }
}

void MainClass::areaGot(int x, int y, int w, int h)
{
    areaScreener->deleteLater();
    screen(x, y, w, h);
}

MainClass::~MainClass()
{
    delete ui;
    delete trayIcon;
}

void MainClass::on_quality_valueChanged(int value)
{
    GLOBAL::quality = value;
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
    QDesktopServices::openUrl(QUrl("http://nikdiamond.hol.es/account/signup.php"));
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
            QFile configFile(QCoreApplication::applicationDirPath() + "\\auth.xml");
            qDebug() << "AuthFile created: " << configFile.fileName();
            if(configFile.open(QIODevice::WriteOnly)){
                QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&configFile);
                xmlWriter->writeStartDocument();
                    xmlWriter->writeStartElement("auth");
                        xmlWriter->writeStartElement("email");
                            xmlWriter->writeCharacters(_email);
                        xmlWriter->writeEndElement();
                        xmlWriter->writeStartElement("password");
                            xmlWriter->writeCharacters(_password);
                        xmlWriter->writeEndElement();
                    xmlWriter->writeEndElement();
                xmlWriter->writeEndDocument();
            }else{
                qDebug() << "Config file error!";
            }
        }
        ui->accountEmail->setText(_email);
    }
    else{
        qDebug() << "Log In failed..." << rp;
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
    QFile configFile(QCoreApplication::applicationDirPath() + "\\auth.xml");
    configFile.remove();
    qDebug() << "Logged out!";
}
