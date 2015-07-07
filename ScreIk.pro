#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T20:52:47
#
#-------------------------------------------------

QT       += core gui network concurrent multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScreIk
TEMPLATE = app
QMAKE_LFLAGS += -static -static-libgcc

DEFINES  += QT_NO_SSL

SOURCES += main.cpp\
        mainclass.cpp \
    screenclass.cpp \
    global.cpp \
    keyhooker.cpp \
    hookkeyboard.cpp \
    areahook.cpp \
    areascreen.cpp \
    app.cpp

HEADERS  += mainclass.h \
    screenclass.h \
    global.h \
    keyhooker.h \
    hookkeyboard.h \
    areahook.h \
    areascreen.h \
    app.h

FORMS    += mainclass.ui

RESOURCES += \
    icons.qrc \
    img.qrc \
    sound.qrc

RC_FILE = iconset.rc

#win32 { # секция будет работать только в Windows
#    WINSDK_DIR = C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Bin # путь к исполняемым файлам Windows SDK (с mt.exe)
#    WIN_PWD = $$replace(PWD, /, \\) # заменить все '/' на '\\' в строке пути к исполняемому файлу программы
#    OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)
#    QMAKE_POST_LINK = \"$$WINSDK_DIR\\mt.exe\" -manifest \"$$WIN_PWD\\$$basename(TARGET).manifest\" -outputresource:\"$$OUT_PWD_WIN\\${DESTDIR_TARGET}\"\;1 # вызов программы mt.exe после компиляции приложения
#}
