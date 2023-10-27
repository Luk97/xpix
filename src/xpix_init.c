#include "xpix_internal.h"

// Global Variable used literally everywhere
Core CORE = {0};

bool internal_initXConnection() {
    
    CORE.display = XOpenDisplay(NULL);
    if (!CORE.display) {
        fprintf(stderr, "ERROR: could not connect to server %s.\n", DisplayString(CORE.display));
        return false;
    }

    return true;
}


// TODO: create defaults to avoid unsuccessful init and make this functions return a boolean
void xpix_init(int fps, const char *title, unsigned int width, unsigned int height, argb32 background) {
    
    if (!internal_initXConnection()) {
        return;
    }

    // Init time management
    CORE.Time.targetFps = fps > 0 ? fps : DEFAULT_TARGET_FPS;
    CORE.Time.realFps = CORE.Time.targetFps;

    // Init input
    CORE.Keyboard.exitKey = XPIX_KEY_ESCAPE;

    // Init counters
    CORE.buttonCount = 0;
    CORE.subwindowCount = 0;

    

    // Init main window
    CORE.mainWindow.parent = RootWindow(CORE.display, DefaultScreen(CORE.display));


    unsigned long valueMask = CWBackPixel | CWBackPixmap | CWBackingStore | CWSaveUnder | CWOverrideRedirect | CWColormap | CWCursor | CWEventMask;
    XSetWindowAttributes setWindowAttr;
    setWindowAttr.background_pixel = background;
    setWindowAttr.background_pixmap = None;
    setWindowAttr.backing_store = NotUseful;
    setWindowAttr.save_under = False;
    setWindowAttr.override_redirect = False;
    setWindowAttr.colormap = CopyFromParent;
    setWindowAttr.cursor = None;
    setWindowAttr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
    
    CORE.mainWindow.handle = XCreateWindow(
        CORE.display,
        DefaultRootWindow(CORE.display),
        0, 0,
        width, height,
        0,
        DefaultDepth(CORE.display, DefaultScreen(CORE.display)),
        InputOutput,
        DefaultVisual(CORE.display, DefaultScreen(CORE.display)),
        valueMask, &setWindowAttr
    );



    CORE.mainWindow.buffer = XCreatePixmap(
        CORE.display,
        CORE.mainWindow.handle,
        DisplayWidth(CORE.display, DefaultScreen(CORE.display)),
        DisplayHeight(CORE.display, DefaultScreen(CORE.display)),
        DefaultDepth(CORE.display, DefaultScreen(CORE.display))   
    );

    CORE.mainWindow.gc = XCreateGC(CORE.display, CORE.mainWindow.handle, 0, NULL);
    
    CORE.mainWindow.width = width;
    CORE.mainWindow.height = height;
    CORE.mainWindow.background = background;
    CORE.mainWindow.borderWidth = 0;
    CORE.mainWindow.borderColor = BlackPixel(CORE.display, DefaultScreen(CORE.display));
    CORE.mainWindow.visible = true;

    

    XStoreName(CORE.display, CORE.mainWindow.handle, title);
    XSetIconName(CORE.display, CORE.mainWindow.handle, title);

    XClassHint *class_hints = XAllocClassHint();
    if (!class_hints) {
        fprintf(stderr, "ERROR: could not allocate class hints\n");
        return;
    }
    class_hints->res_name = (char *) title;
    class_hints->res_class = (char *) title;
    XSetClassHint(CORE.display, CORE.mainWindow.handle, class_hints);
    XFree(class_hints);

    XMapWindow(CORE.display, CORE.mainWindow.handle);

    // HACK: somehow it takes two frames before the window size is adjusted to the sidebars. Needs further investigations
    xpix_beginFrame();
    xpix_endFrame();
    xpix_beginFrame();
    xpix_endFrame();
}