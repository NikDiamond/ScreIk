#include "global.h"

int GLOBAL::quality = 40;
bool GLOBAL::authorized = false;
QString GLOBAL::domain = "screik.tk";
QString GLOBAL::userAgent = "ScreIk";
HWND GLOBAL::mainId = 0;
bool GLOBAL::debugging = true;

GLOBAL::GLOBAL(QObject *parent) :
    QObject(parent)
{}
