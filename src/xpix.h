#ifndef XPIX_H_
#define XPIX_H_

/*
TODO:
- Text
- Subwindows
- optional: Textures
- Raw Bytes as Input (Array of pixels)
- Shapes: Point, Line, Polygon
- optional: Collisions
- Icon
- Colors
- optional: Pixel Formats
*/

#include "xpix_input.h"
#include <stdbool.h>


#define XPIX_API




typedef unsigned int argb32;
typedef int XPix_ID;

typedef struct {
    int x;
    int y;
} XPix_Point;

typedef struct {
    int x, y; // upper left corner
    unsigned int width;
    unsigned int height;
} XPix_Rect;

typedef struct {
    double x, y; // upperleft corner
    unsigned int radius;
} XPix_Circle;


// CORE
extern void xpix_init(int fps, const char *title, unsigned int width, unsigned int height, argb32 background);
extern void xpix_shutdown();
extern bool xpix_shouldQuit();
extern void xpix_beginFrame();
extern void xpix_endFrame();
extern void xpix_setTargetFps(int fps);
extern int xpix_getTargetFps();
extern int xpix_getFps();
extern float xpix_getFrameTime();
extern int xpix_getScreenWidth();
extern int xpix_getScreenHeight();

extern void xpix_test();

// mouse
extern bool xpix_isMouseButtonPressed(XPix_MouseButton button);
extern bool xpix_isMouseButtonReleased(XPix_MouseButton button);
extern bool xpix_isMouseButtonDown(XPix_MouseButton button);
extern bool xpix_isMouseButtonUp(XPix_MouseButton button);
extern bool xpix_isMouseButtonDragged(XPix_MouseButton button);
extern int xpix_getMouseX();
extern int xpix_getMouseY();
extern XPix_Point xpix_getMousePosition();
extern XPix_Point xpix_getMouseMovement();

// WINDOW
extern void xpix_setBackground(XPix_ID window, argb32 background);
extern argb32 xpix_getBackground(XPix_ID window);
extern int xpix_getWindowWidth();
extern int xpix_getWindowHeight();
extern XPix_ID xpix_createSubwindow(XPix_ID parent, int x, int y, unsigned int width, unsigned int height, argb32 background, unsigned int borderWidth, argb32 borderColor);

// SHAPES
extern void xpix_drawRectangle(int x, int y, unsigned int width, unsigned int height, argb32 color);
extern void xpix_drawRectangleRec(XPix_Rect *rect, argb32 color);
extern void xpix_drawCircle(double x, double y, unsigned int radius, argb32 color);
extern void xpix_drawCircleCirc(XPix_Circle *circle, argb32 color);





/*
**************** KEYBOARD ****************
*/

/**
 * @brief Tells if a given key got pressed in the current frame.
 * 
 * @param key value can be found in "xpix_input.h".
 * 
 * @return true if the provided key just got pressed, false otherwise.
*/
XPIX_API bool xpix_isKeyPressed(XPix_Key key);

/**
 * @brief Tells if a given key got released in the current frame.
 * 
 * @param key value can be found in "xpix_input.h".
 * 
 * @return true if the provided key just got released, false otherwise.
*/
XPIX_API bool xpix_isKeyReleased(XPix_Key key);

/**
 * @brief Tells if a given key is currently pressed down in the current frame.
 * 
 * @param key value can be found in "xpix_input.h".
 * 
 * @return true if the provided key is currently down, false otherwise.
*/
XPIX_API bool xpix_isKeyDown(XPix_Key key);

/**
 * @brief Tells if a given key is currently not pressed down in the current frame.
 * 
 * @param key value can be found in "xpix_input.h".
 * 
 * @return true if the provided key is currently not pressed, false otherwise.
*/
XPIX_API bool xpix_isKeyUp(XPix_Key key);

/**
 * @return the current quit key, that triggers to close the window.
*/
XPIX_API XPix_Key xpix_getQuitKey();

/**
 * @brief Set the quit key that triggers to close the window.
 * If XPIX_KEY_NULL is provided, then there is no key that triggers the quitting.
 * 
 * @param key value can be found in "xpix_input.h".
 * 
 * @return true if the provided key got pressed.
*/
XPIX_API void xpix_setQuitKey(XPix_Key key);





/*
**************** BUTTON ****************
*/

/**
 * @brief Creates a button and puts it on the Window window.
 * 
 * This button is customizable with color, size, position, borderwidth and bordercolor.
 * The callback function gets called, when the left mousebutton is released.
 * 
 * @param callback function pointer with no arguments and no return value.
 * @param parent is the window to which the button belongs. 0 for the main window.
 * @param x x position relative to the Window window.
 * @param y y position relative to the Window window.
 * @param width the width of the button.
 * @param height the height of the button.
 * @param color the background color of the inside of the button.
 * @param borderWidth the width of the border in pixels.
 * @param borderColor the color of the border.
 * 
 * @return the identifier for the button that got created or -1 if an error occured.
*/
XPIX_API XPix_ID xpix_createButton(
    void *callback,
    XPix_ID parent,
    int x, int y,
    unsigned int width, unsigned int height,
    argb32 color,
    unsigned int borderWidth, argb32 borderColor
);

/**
 * @brief Creates a button with default parameters.
 * 
 * The color is greyish, the border width 5 pixels large and the border color is black.
 * 
 * @param callback function pointer with no arguments and no return value.
 * @param parent is the window to which the button belongs. 0 for the main window.
 * @param x x position relative to the Window window.
 * @param y y position relative to the Window window.
 * @param width the width of the button.
 * @param height the height of the button.
 * 
 * @return the identifier for the button that got created or -1 if an error occured.
*/
XPIX_API XPix_ID xpix_createDefaultButton(
    void *callback,
    XPix_ID parent,
    int x, int y,
    unsigned int width, unsigned int height
);

/**
 * @brief Makes the button visible on the Window window.
 * 
 * @param buttonID used to identify the button.
*/
XPIX_API void xpix_showButton(XPix_ID buttonID);

/**
 * @brief Makes the button invisible on the Window window.
 * 
 * @param buttonID used to identify the button.
*/
XPIX_API void xpix_hideButton(XPix_ID buttonID);

#endif // XPIX_H_