#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>

class GLOBAL : public QObject
{
    Q_OBJECT
public:
    explicit GLOBAL(QObject *parent = 0);
    static int quality;
    static bool authorized;
};

#endif // GLOBAL_H
