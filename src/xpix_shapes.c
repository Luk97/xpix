#include "xpix_internal.h"

void xpix_drawRectangle(int x, int y, unsigned int width, unsigned int height, argb32 color) {

    XSetForeground(CORE.display, CORE.mainWindow.gc, color);
    XFillRectangle(
        CORE.display,
        CORE.mainWindow.buffer,
        CORE.mainWindow.gc,
        x, y,
        width, height
    );
}

void xpix_drawRectangleRec(XPix_Rect *rect, argb32 color) {
    // TODO: check if pointer of rect or rect is faster
    xpix_drawRectangle(rect->x, rect->y, rect->width, rect->height, color);
}

void xpix_drawCircle(double x, double y, unsigned int radius, argb32 color) {
    XSetForeground(CORE.display, CORE.mainWindow.gc, color);
    XFillArc(CORE.display, CORE.mainWindow.buffer, CORE.mainWindow.gc, (int) x, (int) y, radius*2, radius*2, 0, 360*64);
}

void xpix_drawCircleCirc(XPix_Circle * circle, argb32 color) {
    xpix_drawCircle(circle->x, circle->y, circle->radius, color);
}