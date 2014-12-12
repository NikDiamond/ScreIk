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
    keyPressed = false;
}

void KeyHooker::setTarget(int id)
{
    this->keyId = id;
}

QString KeyHooker::keyName(int hotkeyId)
{
    switch(hotkeyId){
    case 0:
        return "fullscreen";
        break;
    case 1:
        return "areascreen";
        break;
    case 2:
        return "wndscreen";
        break;
    default:
        return "";
    }
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
        keyPressed = true;
        break;
    }
    this->setText(modText + keyText);
}

void KeyHooker::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    switch(key){
    case KEY_SHIFT:
        shiftFlag = false;
        mods = 0;
        break;
    case KEY_CTRL:
        ctrlFlag = false;
        mods = 0;
        break;
    case KEY_ALT:
        altFlag = false;
        mods = 0;
        break;
    default:
        RegisterHotKey(GLOBAL::mainId, this->keyId, mods, event->nativeVirtualKey());
        qDebug() << mods;
        QSettings settings;
        settings.setValue("hotkeys/"+keyName(this->keyId)+"_mod", mods);
        settings.setValue("hotkeys/"+keyName(this->keyId)+"_key", event->nativeVirtualKey());
        settings.setValue("hotkeys/"+keyName(this->keyId)+"_text", keyText);
        settings.sync();

        keyPressed = false;
        if(!shiftFlag && !ctrlFlag && !altFlag){
            this->setText(QKeySequence(key).toString());
            mods = 0;
        }
        if(mods == 0)
            modText = "";
        break;
    }
    if(!shiftFlag && !ctrlFlag && !altFlag && !keyPressed){
        this->clearFocus();
    }
}

void KeyHooker::unregister()
{
    UnregisterHotKey(GLOBAL::mainId, this->keyId);
    this->setText("");
    QSettings settings;
    settings.remove("hotkeys/"+keyName(this->keyId)+"_mod");
    settings.remove("hotkeys/"+keyName(this->keyId)+"_key");
}
