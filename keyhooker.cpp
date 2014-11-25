#include "keyhooker.h"

#define KEY_SHIFT 16777248
#define KEY_CTRL 16777249
#define KEY_ALT 16777251

KeyHooker::KeyHooker(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(pressed()), this, SLOT(unregister()));
    shiftFlag = false;
    ctrlFlag = false;
    altFlag = false;
}

void KeyHooker::setTarget(int id)
{
    this->keyId = id;
}

void KeyHooker::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(!shiftFlag && !ctrlFlag && !altFlag)
        mods = 0;
    switch(key){
    case KEY_SHIFT:
        shiftFlag = true;
        mods = MOD_SHIFT;
        modText = "Shift + ";
        keyText = "";
        break;
    case KEY_CTRL:
        ctrlFlag = true;
        mods = MOD_CONTROL;
        modText = "Ctrl + ";
        keyText = "";
        break;
    case KEY_ALT:
        altFlag = true;
        mods = MOD_ALT;
        modText = "Alt + ";
        keyText = "";
        break;
    default:
        keyText = QKeySequence(key).toString();
        break;
    }
    this->setText(modText + keyText);
}

void KeyHooker::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "release " << event->modifiers();
    int key = event->key();
    switch(key){
    case KEY_SHIFT:
        shiftFlag = false;
        mods = 0;
        qDebug() << "sr";
        break;
    case KEY_CTRL:
        ctrlFlag = false;
        mods = 0;
        qDebug() << "cr";
        break;
    case KEY_ALT:
        altFlag = false;
        mods = 0;
        qDebug() << "ar";
        break;
    default:
        if(!shiftFlag && !ctrlFlag && !altFlag){
            this->setText(QKeySequence(key).toString());
            mods = 0;
            this->clearFocus();
        }
        bool is = RegisterHotKey(GLOBAL::mainId, this->keyId, mods, event->nativeVirtualKey());
        qDebug() << is << " -reg " << mods << " " << event->nativeVirtualKey();
        if(mods == 0)
            modText = "";
        break;
    }
    if(!shiftFlag && !ctrlFlag && !altFlag){
        this->clearFocus();
    }
}

void KeyHooker::unregister()
{
    bool is = UnregisterHotKey(GLOBAL::mainId, this->keyId);
    qDebug() << is << " -unreg";
}
