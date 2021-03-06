#ifndef KEYHOOKER_H
#define KEYHOOKER_H

#include "global.h"
#include "hookkeyboard.h"

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QSettings>
#include <QKeySequence>
#include <windows.h>

class KeyHooker : public QPushButton
{
    Q_OBJECT
public:
    explicit KeyHooker(QWidget *parent = 0);
    void setTarget(int id);
    QString keyName(int hotkeyId);
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    int keyId;
    bool shiftFlag;
    bool ctrlFlag;
    bool altFlag;
    bool keyPressed;
    UINT mods;
    QString modText;
    QString keyText;
private slots:
    void unregister();
};

#endif // KEYHOOKER_H
