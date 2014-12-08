#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T20:52:47
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScreIk
TEMPLATE = app


SOURCES += main.cpp\
        mainclass.cpp \
    screenclass.cpp \
    global.cpp \
    loginclass.cpp \
    siteopen.cpp \
    keyhooker.cpp \
    hookkeyboard.cpp \
    areahook.cpp \
    areascreen.cpp

HEADERS  += mainclass.h \
    screenclass.h \
    global.h \
    loginclass.h \
    siteopen.h \
    keyhooker.h \
    hookkeyboard.h \
    areahook.h \
    areascreen.h

FORMS    += mainclass.ui

RESOURCES += \
    icons.qrc

win32:RC_FILE = iconset.rc
