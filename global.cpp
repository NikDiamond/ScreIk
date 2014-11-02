#include "global.h"

int GLOBAL::quality = 60;
bool GLOBAL::authorized = false;

GLOBAL::GLOBAL(QObject *parent) :
    QObject(parent)
{}
