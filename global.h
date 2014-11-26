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
    static bool authorized;
    static QString domain;
    static HWND mainId;
};

#endif // GLOBAL_H
