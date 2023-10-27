#include "xpix_internal.h"




void xpix_test() {
    XResizeWindow(CORE.display, CORE.mainWindow.handle, CORE.mainWindow.width, CORE.mainWindow.height+300);
}


void xpix_beginFrame() {
    gettimeofday(&CORE.Time.buffer, NULL);


    // update Keyboard State
    for (size_t i = 0; i < (size_t) MAX_KEYS; ++i) {
        CORE.Keyboard.previousKeyState[i] = CORE.Keyboard.currentKeyState[i];
    }


    CORE.Mouse.previousPosition = CORE.Mouse.currentposition;
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
                CORE.Mouse.currentposition.x = event.xmotion.x;
                CORE.Mouse.currentposition.y = event.xmotion.y;
            } break;

            case ConfigureNotify: {

                // window resized
                if (event.xconfigure.width != (int) CORE.mainWindow.width || event.xconfigure.height != (int) CORE.mainWindow.height) {

                    // update width and height of the window
                    CORE.mainWindow.width = event.xconfigure.width;
                    CORE.mainWindow.height = event.xconfigure.height;
                }
            } break;

            case EnterNotify: {
                CORE.Mouse.pointerGrabbed = true;
            } break;

            case LeaveNotify: {
                CORE.Mouse.pointerGrabbed = false;
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
    struct timeval frame;
    gettimeofday(&frame, NULL);

    frame.tv_usec -= CORE.Time.buffer.tv_usec;
    frame.tv_sec  -= CORE.Time.buffer.tv_sec;
    if (frame.tv_usec < 0) {
        frame.tv_sec -= 1;
        frame.tv_usec += 1000000;
    }

    int frameDurationUSec = frame.tv_usec + frame.tv_sec * 1000000;
    int sleepTimeUSec = 1000000 / CORE.Time.targetFps - frameDurationUSec;

    CORE.Time.realFps = sleepTimeUSec > 0 ? CORE.Time.targetFps : 1000000 / frameDurationUSec;
    CORE.Time.frameTime = (float) 1 / CORE.Time.realFps;
    if (sleepTimeUSec > 0) usleep(sleepTimeUSec);
}

extern bool internal_initXConnection();

int xpix_getScreenWidth() {
    if (!CORE.display) {
        if (!internal_initXConnection()) {
            return 0;
        }
    }

    return DisplayWidth(CORE.display, DefaultScreen(CORE.display));
}

int xpix_getScreenHeight() {
    if (!CORE.display) {
        if (!internal_initXConnection()) {
            return 0;
        }
    }
    
    return DisplayHeight(CORE.display, DefaultScreen(CORE.display));
}


