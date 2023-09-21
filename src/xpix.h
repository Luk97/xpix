#ifndef XPIX_H_
#define XPIX_H_

/*
TODO:
- Text
- Subwindows
- Textures
- Shapes: Point, Line, Polygon
- Collisions
- Icon
- Colors
*/

#include "xpix_input.h"
#include <stdbool.h>



typedef unsigned int argb32;

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
extern void xpix_startFrame();
extern void xpix_endFrame();
extern void xpix_setTargetFps(int fps);
extern int xpix_getTargetFps();
extern int xpix_getRealFps();
extern double xpix_getDeltaTime();

// INPUT
// keyboard
extern bool xpix_isKeyPressed(XPix_Key key);
extern bool xpix_isKeyReleased(XPix_Key key);
extern bool xpix_isKeyDown(XPix_Key key);
extern bool xpix_isKeyUp(XPix_Key key);
extern bool xpix_isKeyHoldDown(XPix_Key key);
extern XPix_Key xpix_getQuitKey();
extern void xpix_setQuitKey(XPix_Key key);
// mouse
extern bool xpix_isMouseButtonPressed(XPix_Button button);
extern bool xpix_isMouseButtonReleased(XPix_Button button);
extern bool xpix_isMouseButtonDown(XPix_Button button);
extern bool xpix_isMouseButtonUp(XPix_Button button);
extern bool xpix_isMouseButtonDragged(XPix_Button button);
extern int xpix_getMouseX();
extern int xpix_getMouseY();
extern XPix_Point xpix_getMousePosition();

// WINDOW
extern void xpix_setBackground(argb32 background);
extern void xpix_showWindow();
extern void xpix_hideWindow();
extern int xpix_getWindowWidth();
extern int xpix_getWindowHeight();

// SHAPES
extern void xpix_drawRectangle(int x, int y, unsigned int width, unsigned int height, argb32 color);
extern void xpix_drawRectangleRec(XPix_Rect *rect, argb32 color);
extern void xpix_drawCircle(double x, double y, unsigned int radius, argb32 color);
extern void xpix_drawCircleCirc(XPix_Circle *circle, argb32 color);

#endif // XPIX_H_