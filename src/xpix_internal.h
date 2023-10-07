#ifndef XPIX_INTERNAL_H_
#define XPIX_INTERNAL_H_

/////////////////////////////////////////////////
//                  INCLUDES                   //
/////////////////////////////////////////////////

// C Runtime
#include <unistd.h>
#include <stdlib.h>
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
#define MAX_SUPPORTED_BUTTONS 512
#define MAX_SUPPORTED_SUBWINDOWS 512

// Maybe mask to identify the object by the value?
#define ID_OFFSET_SUBWINDOW 1000
#define ID_OFFSET_BUTTON    2000

typedef struct {
    Window handle;                                  // handle of the main window
    Pixmap buffer;
    GC gc;                                          // graphical context of the main window
    Window parent;
    unsigned int width;                             // current width of the window
    unsigned int height;                            // current height of the window
    argb32 background;                              // background color of the window
    unsigned int borderWidth;
    argb32 borderColor;
    bool visible;
} _Window;



typedef struct {
    Window handle;
    Pixmap buffer;
    void (*callback)(void);
    _Window *parent;
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    argb32 color;
    argb32 pressedColor;
    unsigned int borderWidth;
    argb32 borderColor;
    bool isPressed;
    bool visible;
} _Button;

// core data struct
typedef struct {

    Display *display;                                   // Connection to XServer
    bool quitting;                                      // quit status

    _Window mainWindow;

    _Window subwindows[MAX_SUPPORTED_SUBWINDOWS];
    size_t subwindowCount;

    _Button buttons[MAX_SUPPORTED_BUTTONS];
    size_t buttonCount;

    struct {
        int targetFps;                                 // target frames per second
        int realFps;                                   // real frames per second
        time_t frameTimeBufferUSec;
        time_t targetFrameTimeUSec;
        double targetDeltaTime;
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

} Core;

typedef enum {
    KEY_NULL    = 0,
    KEY_RELEASE = 1,
    KEY_PRESS   = 2,
} KeyAction;

typedef enum {
    MOUSE_NULL    = 0,
    MOUSE_RELEASE = 1,
    MOUSE_PRESS   = 2,
} MouseAction;

extern Core CORE;

#define RED(c) (c) >> 16
#define GREEN(c) (c) >> 8
#define BLUE(c) (c)
#define ARGB(a, r, g, b) (a) << 24 | (r) << 16 | (g) << 8 | (b)

#endif // XPIX_INTERNAL_H_