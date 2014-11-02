#include "global.h"

int GLOBAL::quality = 0;
bool GLOBAL::authorized = false;

GLOBAL::GLOBAL(QObject *parent) :
    QObject(parent)
{}
