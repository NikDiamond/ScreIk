#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

bool isRunning(const QString &process) {
  QProcess tasklist;
  tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO" << "CSV"
                      << "/FI" << QString("IMAGENAME eq %1").arg(process));
  tasklist.waitForFinished();
  QString output = tasklist.readAllStandardOutput();
  return output.startsWith(QString("\"%1").arg(process));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    while(isRunning("ScreIk.exe")){}

    QFileInfo fileInfo(QCoreApplication::applicationDirPath()+"\\ScreIkUpdate.exe");

    QFile mainFile(fileInfo.path()+"\\ScreIk.exe");
    QFile newFile(fileInfo.path() +"\\ScreIkUpdate.exe");
    if(!newFile.open(QIODevice::ReadOnly)) return 1;
    if(mainFile.open(QIODevice::ReadOnly)){
        mainFile.remove();
        newFile.rename("ScreIk.exe");

        QProcess process;
        process.startDetached("\""+fileInfo.path()+"\\ScreIk.exe\"");
    }

    exit(0);
    return a.exec();
}

