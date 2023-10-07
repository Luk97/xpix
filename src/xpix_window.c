#include "xpix_internal.h"


int xpix_getWindowWidth() {
    // TODO: window as paramter
    return CORE.mainWindow.width;
}

int xpix_getWindowHeight() {
    return CORE.mainWindow.height;
}

void xpix_setBackground(XPix_ID window, argb32 background) {

    if (window == 0) {
        CORE.mainWindow.background = background;
        XSetWindowBackground(CORE.display, CORE.mainWindow.handle, background);
    } else if (window >= ID_OFFSET_SUBWINDOW && window < (int) CORE.subwindowCount + ID_OFFSET_SUBWINDOW) {
        CORE.subwindows[window - ID_OFFSET_SUBWINDOW].background = background;
        XSetWindowBackground(CORE.display, CORE.subwindows[window - ID_OFFSET_SUBWINDOW].handle, background);
    } else {
        // TODO: log error
    }

}

argb32 xpix_getBackground(XPix_ID window) {
    if (window == 0) {
        return CORE.mainWindow.background;
    } else if (window >= ID_OFFSET_SUBWINDOW && window < (int) CORE.subwindowCount + ID_OFFSET_SUBWINDOW) {
        return CORE.subwindows[window - ID_OFFSET_SUBWINDOW].background;
    } else {
        // TODO: log error
        return BlackPixel(CORE.display, DefaultScreen(CORE.display));
    }
}

XPix_ID xpix_createSubwindow(XPix_ID parent, int x, int y, unsigned int width, unsigned int height, argb32 background, unsigned int borderWidth, argb32 borderColor) {

    if (CORE.subwindowCount >= MAX_SUPPORTED_SUBWINDOWS) {
        // TODO: log error
        return -1;
    }

    XPix_ID id = CORE.subwindowCount;
    CORE.subwindowCount += 1;

    if (parent == 0) {
        CORE.subwindows[id].parent = CORE.mainWindow.handle;
    } else if (parent >= ID_OFFSET_SUBWINDOW && parent < (int) CORE.subwindowCount + ID_OFFSET_SUBWINDOW) {
        CORE.subwindows[id].parent = CORE.subwindows[parent - ID_OFFSET_SUBWINDOW].handle;
    } else {
        // TODO: log error
        return -1;
    }

    CORE.subwindows[id].handle = XCreateSimpleWindow(
        CORE.display,
        CORE.subwindows[id].parent,
        x, y,
        width, height,
        borderWidth, borderColor,
        background
    );

    CORE.subwindows[id].buffer = XCreatePixmap(
        CORE.display,
        CORE.subwindows[id].handle,
        width, height,
        DefaultDepth(CORE.display, DefaultScreen(CORE.display))
    );

    CORE.subwindows[id].gc = XCreateGC(
        CORE.display,
        CORE.subwindows[id].buffer,
        0, NULL // options
    );

    CORE.subwindows[id].width = width;
    CORE.subwindows[id].height = height;
    CORE.subwindows[id].background = background;
    CORE.subwindows[id].borderWidth = borderWidth;
    CORE.subwindows[id].borderColor = borderColor;
    CORE.subwindows[id].visible = true;

    // TODO: add all the event masks
    XSelectInput(CORE.display, CORE.subwindows[id].handle, ButtonPressMask | ButtonReleaseMask);
    XMapWindow(CORE.display, CORE.subwindows[id].handle);

    return id + ID_OFFSET_SUBWINDOW;
}
