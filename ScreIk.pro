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
    areaclass.cpp \
    drawerclass.cpp \
    siteopen.cpp \
    keyhooker.cpp \
    hookkeyboard.cpp

HEADERS  += mainclass.h \
    screenclass.h \
    global.h \
    loginclass.h \
    areaclass.h \
    drawerclass.h \
    siteopen.h \
    keyhooker.h \
    hookkeyboard.h

FORMS    += mainclass.ui

RESOURCES += \
    icons.qrc
