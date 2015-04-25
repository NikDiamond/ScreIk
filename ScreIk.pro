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

win32:RC_FILE = iconset.rc
