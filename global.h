#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QString>

#include <windows.h>

class GLOBAL : public QObject
{
    Q_OBJECT
public:
    explicit GLOBAL(QObject *parent = 0);
    static int quality;
    static int localQuality;
    static bool useBufferPix;

    static bool authorized;
    static QString domain;
    static QString userAgent;
    static HWND mainId;
    static bool debugging;
    static QString version;
};

#endif // GLOBAL_H
