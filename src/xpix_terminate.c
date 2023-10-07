#include "xpix_internal.h"

void xpix_shutdown() {
    XDestroyWindow(CORE.display, CORE.mainWindow.handle);
    XFreePixmap(CORE.display, CORE.mainWindow.buffer);
    XFreeGC(CORE.display, CORE.mainWindow.gc);

    // TODO: free all memory allocated by malloc

    XCloseDisplay(CORE.display);
}

bool xpix_shouldQuit() {
    return CORE.quitting;
}