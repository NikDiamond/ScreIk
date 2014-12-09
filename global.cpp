#include "global.h"

int GLOBAL::quality = 40;
bool GLOBAL::authorized = false;
QString GLOBAL::domain = "screik.tk";
HWND GLOBAL::mainId = 0;

GLOBAL::GLOBAL(QObject *parent) :
    QObject(parent)
{}
