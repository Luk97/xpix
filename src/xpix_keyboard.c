#include "xpix_internal.h"

bool xpix_isKeyPressed(XPix_Key key) {  
    if (key > 0 && key < MAX_KEYS) {
        if (CORE.Keyboard.previousKeyState[key] == 0 && CORE.Keyboard.currentKeyState[key] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isKeyReleased(XPix_Key key) {
    if (key > 0 && key < MAX_KEYS) {
        if (CORE.Keyboard.previousKeyState[key] == 1 && CORE.Keyboard.currentKeyState[key] == 0) {
            return true;
        }
    }
    return false;
}

bool xpix_isKeyDown(XPix_Key key) {
    if (key > 0 && key < MAX_KEYS) {
        if (CORE.Keyboard.currentKeyState[key] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isKeyUp(XPix_Key key) {
    if (key > 0 && key < MAX_KEYS) {
        if (CORE.Keyboard.currentKeyState[key] == 0) {
            return true;
        }
    }
    return false;
}


bool xpix_isKeyHoldDown(XPix_Key key) {
    if (key > 0 && key < MAX_KEYS) {
        if (CORE.Keyboard.previousKeyState[key] == 1 && CORE.Keyboard.currentKeyState[key] == 1) {
            return true;
        }
    }
    return false;
}

void xpix_setQuitKey(XPix_Key key) {
    // key == 0 if no key should trigger quitting the application
    if (key >= 0 && key < MAX_KEYS) {
        CORE.Keyboard.exitKey = key;
    }
}

XPix_Key xpix_getQuitKey() {
    return CORE.Keyboard.exitKey;
}