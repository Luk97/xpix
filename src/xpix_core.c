/////////////////////////////////////////////////
//                  INCLUDES                   //
/////////////////////////////////////////////////

// C Runtime
#include <unistd.h>
#include <stdio.h>
#include <time.h>
//#include <stdlib.h>

// X11 libs
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

// xpix Header
#include "xpix.h"


/////////////////////////////////////////////////
//                DEFINITIONS                  //
/////////////////////////////////////////////////

#define false 0
#define true 1

#define DEFAULT_TARGET_FPS 60                           // fallback if no valid fps are provided

#define MAX_KEYS 512                                    // number of supported key
#define MAX_MOUSE_BUTTONS 3                             // number of supported mouse buttons 

// core data struct
typedef struct {

    Display *display;                                   // Connection to XServer
    bool visible;                                       // visible status
    bool quitting;                                      // quit status

    struct {
        int targetFps;                                 // target frames per second
        int realFps;                                   // real frames per second
        double targetFrameTime;
        clock_t frameTimeBuffer;
        double deltaTime;
    } Time;

    struct {
        char previousKeyState[MAX_KEYS];                // key states from previous frame
        char currentKeyState[MAX_KEYS];                 // key states from current frame
        int exitKey;                                    // key that triggers exitting the application
    } Keyboard;

    struct {
        XPix_Point position;                            // current position of mouse in the window
        char previousButtonState[MAX_MOUSE_BUTTONS];    // mouse button states from previous frame
        char currentButtonState[MAX_MOUSE_BUTTONS];     // mouse button states from current frame
    } Mouse;

    struct {
        Window handle;                                  // handle of the main window
        Pixmap buffer;                                  // buffer in which the pixels get stored
        GC gc;                                          // graphical context of the main window
        unsigned int width;                             // current width of the window
        unsigned int height;                            // current height of the window
        // TODO: different pixel formats
        argb32 background;                              // background color of the window
    } Window;

} Data;

typedef enum {
    KEY_RELEASE = 0,
    KEY_PRESS   = 1,
} KeyAction;

typedef enum {
    MOUSE_RELEASE = 0,
    MOUSE_PRESS   = 1,
} MouseAction;

// Global Variables
static Data CORE = {0};


/////////////////////////////////////////////////
//                    CORE                     //
/////////////////////////////////////////////////

void xpix_init(int fps, const char *title, unsigned int width, unsigned int height, argb32 background) {
    CORE.display = XOpenDisplay(NULL);
    if (!CORE.display) {
        fprintf(stderr, "ERROR: could not connect to server %s.\n", DisplayString(CORE.display));
    }

    CORE.Keyboard.exitKey = XPIX_KEY_ESCAPE;

    CORE.Time.targetFps = fps > 0 ? fps : DEFAULT_TARGET_FPS;
    CORE.Time.targetFrameTime = 1 / (double) CORE.Time.targetFps;

    // Window
    CORE.Window.handle = XCreateSimpleWindow(
                        CORE.display,
                        RootWindow(CORE.display, DefaultScreen(CORE.display)),
                        0, 0, // Relative Coordinates of upper left corner
                        width, height,
                        0, 0, // Border and Border Color
                        background
                    );
    CORE.Window.buffer = XCreatePixmap(
                        CORE.display, CORE.Window.handle,
                        DisplayWidth(CORE.display, DefaultScreen(CORE.display)),
                        DisplayHeight(CORE.display, DefaultScreen(CORE.display)),
                        DefaultDepth(CORE.display, DefaultScreen(CORE.display))
                    );
    CORE.Window.gc = XCreateGC(CORE.display, CORE.Window.buffer, 0, NULL);
    CORE.Window.width = width;
    CORE.Window.height = height;
    CORE.Window.background = background;
    XStoreName(CORE.display, CORE.Window.handle, title);
    XSelectInput(CORE.display, CORE.Window.handle,
        ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(CORE.display, CORE.Window.handle);
    CORE.visible = true;
}

void xpix_shutdown() {
    XDestroyWindow(CORE.display, CORE.Window.handle);
    XFreePixmap(CORE.display, CORE.Window.buffer);
    XFreeGC(CORE.display, CORE.Window.gc);

    XCloseDisplay(CORE.display);
}

bool xpix_shouldQuit() {
    return CORE.quitting;
}

void xpix_setTargetFps(int fps) {
    // TODO: log invalid fps
    if (fps > 0) {
        CORE.Time.targetFps = fps;
    }
}

int xpix_getTargetFps() {
    return CORE.Time.targetFps;
}

int xpix_getRealFps() {
    return CORE.Time.realFps;
}

extern double xpix_getDeltaTime() {
    return CORE.Time.deltaTime;
}


/////////////////////////////////////////////////
//                    KEYS                     //
/////////////////////////////////////////////////

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


/////////////////////////////////////////////////
//                   MOUSE                     //
/////////////////////////////////////////////////

bool xpix_isMouseButtonPressed(XPix_Button button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 0 && CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonReleased(XPix_Button button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 1 && CORE.Mouse.currentButtonState[button] == 0) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonDown(XPix_Button button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonUp(XPix_Button button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.currentButtonState[button] == 0) {
            return true;
        }
    }
    return false;
}

bool xpix_isMouseButtonDragged(XPix_Button button) {
    if (button > 0 && button < MAX_MOUSE_BUTTONS) {
        if (CORE.Mouse.previousButtonState[button] == 1 && CORE.Mouse.currentButtonState[button] == 1) {
            return true;
        }
    }
    return false;
}

int xpix_getMouseX() {
    return CORE.Mouse.position.x;
}

int xpix_getMouseY() {
    return CORE.Mouse.position.y;
}

XPix_Point xpix_getMousePosition() {
    return CORE.Mouse.position;
}


/////////////////////////////////////////////////
//                 CALLBACKS                   //
/////////////////////////////////////////////////

void internal_keyCallback(XKeyEvent event, KeyAction action) {

    KeySym key_sym;
    XLookupString(&event, NULL, 0, &key_sym, NULL);

    int key_index = (int) key_sym;

    // case Escape
    // TODO: translation function and check if everywhere appliable
    if (key_index == 65307) {
        key_index = 27;
    }

    if (key_index > 0 && key_index < MAX_KEYS) {

        // Check if lower case letter
        if (key_index >= 'a' && key_index <= 'z') {
            key_index -= 32;
        }
        
        // Check if exit key has been pressed
        if (key_index == CORE.Keyboard.exitKey) {
            CORE.quitting = true;
        }

        if (action == KEY_PRESS) {
            CORE.Keyboard.currentKeyState[key_index] = 1;
        } else if (action == KEY_RELEASE) {
            CORE.Keyboard.currentKeyState[key_index] = 0;
        }
    }
}

void internal_buttonCallback(XButtonEvent event, MouseAction action) {
    int mouse_index = (int) event.button;

    if (mouse_index > 0 && mouse_index < MAX_MOUSE_BUTTONS) {
        if (action == MOUSE_PRESS) {
            CORE.Mouse.currentButtonState[mouse_index] = 1;
        } else if (action == MOUSE_RELEASE) {
            CORE.Mouse.currentButtonState[mouse_index] = 0;
        }
    }
}

void internal_motionCallback(XMotionEvent event) {
    CORE.Mouse.position.x = event.x;
    CORE.Mouse.position.y = event.y;
}

void internal_resizeCallback(XConfigureEvent event) {
    CORE.Window.width = event.width;
    CORE.Window.height = event.height;
}

/////////////////////////////////////////////////
//                   FRAMES                    //
/////////////////////////////////////////////////

void internal_buffer2Window() {
    XCopyArea(
        CORE.display,
        CORE.Window.buffer,
        CORE.Window.handle,
        CORE.Window.gc, 
        0, 0, // src x and y
        CORE.Window.width, CORE.Window.height,
        0, 0 // dest x and y
    );
    // TODO: check if xflush is necessary
    XFlush(CORE.display);
}

void internal_updateKeyStates() {
    for (size_t i = 0; i < (size_t) MAX_KEYS; ++i) {
        CORE.Keyboard.previousKeyState[i] = CORE.Keyboard.currentKeyState[i];
    }
}

void internal_updateMouseStates() {
    for (size_t i = 0; i < (size_t) MAX_MOUSE_BUTTONS; ++i) {
        CORE.Mouse.previousButtonState[i] = CORE.Mouse.currentButtonState[i];
    }
}

void xpix_startFrame() {

    CORE.Time.frameTimeBuffer = clock();
    
    internal_updateKeyStates();
    internal_updateMouseStates();

    while (XPending(CORE.display)) {

        XEvent event;
        XNextEvent(CORE.display, &event);

        switch (event.type) {

            case KeyPress: {
                internal_keyCallback(event.xkey, KEY_PRESS);
            } break;

            case KeyRelease: {
                internal_keyCallback(event.xkey, KEY_RELEASE);
            } break;

            case ButtonPress: {
                internal_buttonCallback(event.xbutton, MOUSE_PRESS);
            } break;

            case ButtonRelease: {
                internal_buttonCallback(event.xbutton, MOUSE_RELEASE);
            } break;

            case MotionNotify: {
                internal_motionCallback(event.xmotion);
            }break;

            case Expose: {
                internal_buffer2Window();
            } break;

            case ConfigureNotify: {
                internal_resizeCallback(event.xconfigure);
            } break;

            default: break;
        }
    }

    // refresh window with background color
    XSetForeground(CORE.display, CORE.Window.gc, CORE.Window.background);
    XFillRectangle(
        CORE.display, CORE.Window.buffer, CORE.Window.gc,
        0, 0, // x and y
        CORE.Window.width, CORE.Window.height
    );
}

void xpix_endFrame() {
    internal_buffer2Window();

    // Time handling
    double realFrameTime = (double) (clock() - CORE.Time.frameTimeBuffer) / CLOCKS_PER_SEC;

    double waitingTime = CORE.Time.targetFrameTime - realFrameTime;
    if (waitingTime < 0) {
        // real fps gets used because the frame took longer than the target fps aims for
        waitingTime = 0;
        CORE.Time.realFps = (int) (1 / realFrameTime);
        CORE.Time.deltaTime = realFrameTime;
    } else {
        // target fps is usable because the frame got calculated fast enough
        CORE.Time.realFps = CORE.Time.targetFps;
        CORE.Time.deltaTime = CORE.Time.targetFrameTime;
    }

    usleep(waitingTime * CLOCKS_PER_SEC);
}

void xpix_setBackground(argb32 background) {
    CORE.Window.background = background;
    XSetWindowBackground(CORE.display, CORE.Window.handle, background);
}

void xpix_showWindow() {
    XMapWindow(CORE.display, CORE.Window.handle);
    CORE.visible = true;
}

void xpix_hideWindow() {
    XUnmapWindow(CORE.display, CORE.Window.handle);
    CORE.visible = false;
}


int xpix_getWindowWidth() {
    return CORE.Window.width;
}

int xpix_getWindowHeight() {
    return CORE.Window.height;
}

void xpix_drawRectangle(int x, int y, unsigned int width, unsigned int height, argb32 color) {
    XSetForeground(CORE.display, CORE.Window.gc, color);
    XFillRectangle(CORE.display, CORE.Window.buffer, CORE.Window.gc, x, y, width, height);
}

void xpix_drawRectangleRec(XPix_Rect *rect, argb32 color) {
    // TODO: check if pointer of rect or rect is faster
    xpix_drawRectangle(rect->x, rect->y, rect->width, rect->height, color);
}

void xpix_drawCircle(double x, double y, unsigned int radius, argb32 color) {
    XSetForeground(CORE.display, CORE.Window.gc, color);
    XFillArc(CORE.display, CORE.Window.buffer, CORE.Window.gc, (int) x, (int) y, radius*2, radius*2, 0, 360*64);
}

void xpix_drawCircleCirc(XPix_Circle * circle, argb32 color) {
    xpix_drawCircle(circle->x, circle->y, circle->radius, color);
}