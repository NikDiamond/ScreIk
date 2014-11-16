#include "global.h"

int GLOBAL::quality = 60;
bool GLOBAL::authorized = false;
QString GLOBAL::domain = "nikdiamond.hol.es";

GLOBAL::GLOBAL(QObject *parent) :
    QObject(parent)
{}
