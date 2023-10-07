#include "xpix_internal.h"

void xpix_init(int fps, const char *title, unsigned int width, unsigned int height, argb32 background) {
    
    // Connect to X Server
    CORE.display = XOpenDisplay(NULL);
    if (!CORE.display) {
        fprintf(stderr, "ERROR: could not connect to server %s.\n", DisplayString(CORE.display));
        return;
    }

    // Init input
    CORE.Keyboard.exitKey = XPIX_KEY_ESCAPE;

    // Init counters
    CORE.buttonCount = 0;
    CORE.subwindowCount = 0;

    // Init time management
    CORE.Time.targetFps = fps > 0 ? fps : DEFAULT_TARGET_FPS;
    CORE.Time.realFps = CORE.Time.targetFps;
    CORE.Time.targetFrameTimeUSec = 1000000 / CORE.Time.targetFps;
    CORE.Time.targetDeltaTime = (double) 1 / CORE.Time.targetFps;

    // Init main window
    CORE.mainWindow.parent = RootWindow(CORE.display, DefaultScreen(CORE.display));

    CORE.mainWindow.handle = XCreateSimpleWindow(
        CORE.display,
        CORE.mainWindow.parent,
        0, 0, // Relative x and y coordinates of upper left corner
        width, height,
        0, 0, // Borderwidth and bordercolor
        background
    );

    CORE.mainWindow.buffer = XCreatePixmap(
        CORE.display,
        CORE.mainWindow.handle,
        width, height,
        DefaultDepth(CORE.display, DefaultScreen(CORE.display))   
    );

    CORE.mainWindow.gc = XCreateGC(CORE.display, CORE.mainWindow.handle, 0, NULL);
    
    CORE.mainWindow.width = width;
    CORE.mainWindow.height = height;
    CORE.mainWindow.background = background;
    CORE.mainWindow.borderWidth = 0;
    CORE.mainWindow.borderColor = BlackPixel(CORE.display, DefaultScreen(CORE.display));
    CORE.mainWindow.visible = true;

    // Transfer main window data to X Server
    XStoreName(CORE.display, CORE.mainWindow.handle, title);
    XSelectInput(CORE.display, CORE.mainWindow.handle,
        KeyPressMask | KeyReleaseMask | ButtonPressMask |
        ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(CORE.display, CORE.mainWindow.handle);
}