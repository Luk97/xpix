#include "xpix_internal.h"

bool xpix_isMouseButtonPressed(XPix_MouseButton button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 0 && CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonReleased(XPix_MouseButton button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 1 && CORE.Mouse.currentButtonState[button] == 0) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonDown(XPix_MouseButton button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonUp(XPix_MouseButton button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.currentButtonState[button] == 0) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonDragged(XPix_MouseButton button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 1 && CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

int xpix_getMouseX() {
    return CORE.Mouse.currentposition.x;
}

int xpix_getMouseY() {
    return CORE.Mouse.currentposition.y;
}

XPix_Point xpix_getMousePosition() {
    return CORE.Mouse.currentposition;
}

XPix_Point xpix_getMouseMovement() {
    XPix_Point movement = {
        .x = CORE.Mouse.currentposition.x - CORE.Mouse.previousPosition.x,
        .y = CORE.Mouse.currentposition.y - CORE.Mouse.previousPosition.y
    };
    return movement;
}
