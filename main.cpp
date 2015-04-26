#include "mainclass.h"
#include <windows.h>
#include <QApplication>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QCoreApplication::setOrganizationName(QObject::tr("NikDiamond"));
    QCoreApplication::setApplicationName(QObject::tr("ScreIk"));

    MainClass w;
    w.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    return a.exec();
}
