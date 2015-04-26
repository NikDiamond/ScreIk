#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T15:11:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScreIkUpdater
TEMPLATE = app


SOURCES += main.cpp

win32 { # секция будет работать только в Windows
    WINSDK_DIR = C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Bin # путь к исполняемым файлам Windows SDK (с mt.exe)
    WIN_PWD = $$replace(PWD, /, \\) # заменить все '/' на '\\' в строке пути к исполняемому файлу программы
    OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)
    QMAKE_POST_LINK = \"$$WINSDK_DIR\\mt.exe\" -manifest \"$$WIN_PWD\\$$basename(TARGET).manifest\" -outputresource:\"$$OUT_PWD_WIN\\${DESTDIR_TARGET}\"\;1 # вызов программы mt.exe после компиляции приложения
}
