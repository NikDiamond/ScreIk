#include "global.h"

int GLOBAL::quality = 40;
HWND GLOBAL::mainId = 0;

bool GLOBAL::authorized = false;
bool GLOBAL::debugging  = false;

QString GLOBAL::domain    = "screik.tk";
QString GLOBAL::userAgent = "ScreIk";
QString GLOBAL::version   = "0.9.8.5b";

GLOBAL::GLOBAL(QObject *parent):QObject(parent){}
