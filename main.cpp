#include "mainclass.h"
#include "global.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(false);
    QCoreApplication::setApplicationName(QObject::tr("ScreIk"));
    QCoreApplication::setApplicationVersion(GLOBAL::version);
    QCoreApplication::setOrganizationName(QObject::tr("NikD Projects"));
    QCoreApplication::setOrganizationDomain("nikd.tk");

    MainClass w;
    w.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

    return a.exec();
}
