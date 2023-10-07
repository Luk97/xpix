#include "xpix_internal.h"

/*
TODO:
- rgb to hsv, because then the button can be changed by brightness
- text for buttons
- right click info
- image for buttons
- motion from mouse triggers color change
*/

#define DEFAULT_COLOR 0xFFAAAAAA
#define DEFAULT_BORDER_WIDTH 5
#define DEFAULT_BORDER_COLOR 0xFF000000

#define PRESS_OFFSET 50


XPix_ID xpix_createButton(void *callback, XPix_ID parent, int x, int y, unsigned int width, unsigned int height, argb32 color, unsigned int borderWidth, argb32 borderColor) {

    if (CORE.buttonCount >= MAX_SUPPORTED_BUTTONS) {
        // TODO: log error
        return -1;
    }

    XPix_ID id = CORE.buttonCount;
    CORE.buttonCount += 1;

    if (parent == 0) {
        CORE.buttons[id].parent = &CORE.mainWindow;
    } else if (parent >= ID_OFFSET_SUBWINDOW && parent < (int) CORE.subwindowCount + ID_OFFSET_SUBWINDOW) {
        CORE.buttons[id].parent = &CORE.subwindows[parent - ID_OFFSET_SUBWINDOW];
    } else {
        // TODO: log error
        return -1;
    }

    CORE.buttons[id].handle = XCreateSimpleWindow(
        CORE.display,
        CORE.buttons[id].parent->handle,
        x, y,
        width, height,
        borderWidth, borderColor,
        color
    );

    CORE.buttons[id].buffer = XCreatePixmap(
        CORE.display,
        CORE.buttons[id].handle,
        width, height,
        DefaultDepth(CORE.display, DefaultScreen(CORE.display))
    );

    CORE.buttons[id].callback = callback;
    CORE.buttons[id].x = x;
    CORE.buttons[id].y = y;
    CORE.buttons[id].width = width;
    CORE.buttons[id].height = height;
    CORE.buttons[id].color = color;
    CORE.buttons[id].borderWidth = borderWidth;
    CORE.buttons[id].borderColor = borderColor;
    CORE.buttons[id].visible = true;
    CORE.buttons[id].isPressed = false;

    unsigned char r = RED(CORE.buttons[id].color);
    unsigned char g = GREEN(CORE.buttons[id].color);
    unsigned char b = BLUE(CORE.buttons[id].color);
    
    if (r + PRESS_OFFSET < 256) {
        r += PRESS_OFFSET;
    }

    if (g + PRESS_OFFSET < 256) {
        g += PRESS_OFFSET;
    }

    if (b + PRESS_OFFSET < 256) {
        b += PRESS_OFFSET;
    }

    CORE.buttons[id].pressedColor = ARGB(0xFF, r, g, b);

    XSelectInput(CORE.display, CORE.buttons[id].handle, ButtonPressMask | ButtonReleaseMask);
    XMapWindow(CORE.display, CORE.buttons[id].handle);

    return id + ID_OFFSET_BUTTON;
}

XPix_ID xpix_createDefaultButton(void *callback, XPix_ID parent, int x, int y, unsigned int width, unsigned int height) {
    return xpix_createButton(callback, parent, x, y, width, height, DEFAULT_COLOR, DEFAULT_BORDER_WIDTH, DEFAULT_BORDER_COLOR);
}


void xpix_showButton(XPix_ID buttonID) {
    if (buttonID >= ID_OFFSET_BUTTON && buttonID < (int) CORE.buttonCount + ID_OFFSET_BUTTON && !CORE.buttons[buttonID].visible) {
        CORE.buttons[buttonID].visible = true;
        XMapWindow(CORE.display, CORE.buttons[buttonID].handle);
    }
}

void xpix_hideButton(XPix_ID buttonID) {
    if (buttonID >= ID_OFFSET_BUTTON && buttonID < (int) CORE.buttonCount + ID_OFFSET_BUTTON && CORE.buttons[buttonID].visible) {
        CORE.buttons[buttonID].visible = false;
        XUnmapWindow(CORE.display, CORE.buttons[buttonID].handle);
    }
}
