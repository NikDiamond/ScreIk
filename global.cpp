#include "global.h"

int GLOBAL::quality = 40;
int GLOBAL::localQuality = 40;
bool GLOBAL::useBufferPix = true;

HWND GLOBAL::mainId = 0;

bool GLOBAL::authorized = false;
bool GLOBAL::debugging  = true;

QString GLOBAL::domain    = "screik.tk";
QString GLOBAL::userAgent = "ScreIk";
QString GLOBAL::version   = "0.9.9b";

GLOBAL::GLOBAL(QObject *parent):QObject(parent){}
