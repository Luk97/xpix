#include "xpix_internal.h"

#include <wchar.h>


// Global Variable used literally everywhere
Core CORE = {0};



void xpix_beginFrame() {

    CORE.Time.frameTimeBufferUSec = clock();


    // update Keyboard State
    for (size_t i = 0; i < (size_t) MAX_KEYS; ++i) {
        CORE.Keyboard.previousKeyState[i] = CORE.Keyboard.currentKeyState[i];
    }

    // update Mousebutton State
    for (size_t i = 0; i < (size_t) MAX_MOUSE_BUTTONS; ++i) {
        CORE.Mouse.previousButtonState[i] = CORE.Mouse.currentButtonState[i];
    }

    while (XPending(CORE.display)) {

        XEvent event;
        XNextEvent(CORE.display, &event);
        
        KeyAction keyAction = KEY_NULL;
        MouseAction mouseAction = MOUSE_NULL;

        switch (event.type) {

            case KeyPress: {
                // fall through to KeyRelease
                keyAction = KEY_PRESS;
            }

            case KeyRelease: {
                
                if (keyAction == KEY_NULL) {
                    keyAction = KEY_RELEASE;
                }

                KeySym key_sym;
                XLookupString(&event.xkey, NULL, 0, &key_sym, NULL);

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

                    if (keyAction == KEY_PRESS) {
                        CORE.Keyboard.currentKeyState[key_index] = 1;
                    } else if (keyAction == KEY_RELEASE) {
                        CORE.Keyboard.currentKeyState[key_index] = 0;
                    }
                }

            } break;

            case ButtonPress: {
                // fall through to ButtonRelease
                mouseAction = MOUSE_PRESS;
            }

            case ButtonRelease: {

                if (mouseAction == MOUSE_NULL) {
                    mouseAction = MOUSE_RELEASE;
                }

                // check for Window window
                if (event.xbutton.window == CORE.mainWindow.handle) {

                    if (event.xbutton.button > 0 && event.xbutton.button < MAX_MOUSE_BUTTONS) {
                        if (mouseAction == MOUSE_PRESS) {
                            CORE.Mouse.currentButtonState[event.xbutton.button] = 1;
                        } else if (mouseAction == MOUSE_RELEASE) {
                            CORE.Mouse.currentButtonState[event.xbutton.button] = 0;
                        }
                    }
                }

                // check for buttons
                else {
                    for (size_t i = 0; i < CORE.buttonCount; ++i) {
                        if (event.xbutton.window == CORE.buttons[i].handle) {
                            
                            if (event.xbutton.button == XPIX_MOUSE_LEFT) {
                                if (mouseAction == MOUSE_PRESS) {
                                    CORE.buttons[i].isPressed = true;

                                } else if (mouseAction == MOUSE_RELEASE) {
                                    CORE.buttons[i].isPressed = false;
                                    (*CORE.buttons[i].callback)();
                                }
                            }
                        }
                    }
                }
            } break;

            case MotionNotify: {
                
                // update mouse positions on screen
                CORE.Mouse.position.x = event.xmotion.x;
                CORE.Mouse.position.y = event.xmotion.y;
            } break;

            case ConfigureNotify: {

                // window resized
                if (event.xconfigure.width != (int) CORE.mainWindow.width ||
                    event.xconfigure.height != (int) CORE.mainWindow.height) {

                    // resize buffer
                    XFreePixmap(CORE.display, CORE.mainWindow.buffer);
                    CORE.mainWindow.buffer = XCreatePixmap(
                        CORE.display,
                        CORE.mainWindow.handle,
                        event.xconfigure.width,
                        event.xconfigure.height,
                        DefaultDepth(CORE.display, DefaultScreen(CORE.display))
                    );

                    // update width and height of the window
                    CORE.mainWindow.width = event.xconfigure.width;
                    CORE.mainWindow.height = event.xconfigure.height;
                }
            } break;

            default: break;
        }
    }

    // clear buffer with background color
    XSetForeground(CORE.display, CORE.mainWindow.gc, CORE.mainWindow.background);
    XFillRectangle(
        CORE.display,
        CORE.mainWindow.buffer,
        CORE.mainWindow.gc,
        0, 0, // x and y pos
        CORE.mainWindow.width,
        CORE.mainWindow.height
    );

    // refresh visible subwindows
    for (size_t i = 0; i < CORE.subwindowCount; ++i) {
        if (CORE.subwindows[i].visible) {
            XSetForeground(
                CORE.display,
                CORE.subwindows[i].gc,
                CORE.subwindows[i].background
            );
            XFillRectangle(
                CORE.display, CORE.subwindows[i].buffer, CORE.subwindows[i].gc,
                0, 0,
                CORE.subwindows[i].width, CORE.subwindows[i].height
            );
        }
    }

    // refresh visible buttons
    for (size_t i = 0; i < CORE.buttonCount; ++i) {
        if (CORE.buttons[i].visible) {
            XSetForeground(
                CORE.display,
                CORE.buttons[i].parent->gc,
                CORE.buttons[i].isPressed ? CORE.buttons[i].pressedColor : CORE.buttons[i].color
            );
            XFillRectangle(
                CORE.display, CORE.buttons[i].buffer, CORE.buttons[i].parent->gc,
                0, 0,
                CORE.buttons[i].width, CORE.buttons[i].height
            );
        }
    }
}



void xpix_endFrame() {

    // copy window buffer to window
    XCopyArea(
        CORE.display,
        CORE.mainWindow.buffer,
        CORE.mainWindow.handle,
        CORE.mainWindow.gc,
        0, 0,   // src x and y
        CORE.mainWindow.width,
        CORE.mainWindow.height,
        0, 0    // dst x and y
    );

    // draw subwindows
    for (size_t i = 0; i < CORE.subwindowCount; ++i) {
        if (CORE.subwindows[i].visible) {
            XCopyArea(
                CORE.display,
                CORE.subwindows[i].buffer,
                CORE.subwindows[i].handle,
                CORE.subwindows[i].gc, 
                0, 0, // src x and y
                CORE.subwindows[i].width, CORE.subwindows[i].height,
                0, 0 // dest x and y
            );
        }
    }

    // draw buttons
    for (size_t i = 0; i < CORE.buttonCount; ++i) {
        if (CORE.buttons[i].visible) {
            XCopyArea(
                CORE.display,
                CORE.buttons[i].buffer,
                CORE.buttons[i].handle,
                CORE.buttons[i].parent->gc, 
                0, 0, // src x and y
                CORE.buttons[i].width, CORE.buttons[i].height,
                0, 0 // dest x and y
            );
        }
    }

    // Time handling
    time_t realFrameTimeUSec = clock() - CORE.Time.frameTimeBufferUSec;
    int waitingTimeUSec = CORE.Time.targetFrameTimeUSec - realFrameTimeUSec;

    // target framerate achieved
    if (waitingTimeUSec > 0) {
        CORE.Time.realFps = CORE.Time.targetFps;
        CORE.Time.deltaTime = CORE.Time.targetDeltaTime;
        usleep(waitingTimeUSec);
    }
    // frame overshoots target framerate
    else {
        CORE.Time.realFps = 1000000 / realFrameTimeUSec;
        CORE.Time.deltaTime = (double) 1 / CORE.Time.realFps;
    }
}



