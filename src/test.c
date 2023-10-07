#include "xpix.h"

#include <unistd.h>
#include <stdio.h>

#define PROFILER_IMPLEMENTATION
#include "../thirdparty/profiler.h"

#define pink 0xFFFFAAAA
#define blue 0xFFAAAAFF
#define black 0xFF000000
#define FPS 60

void callback() {
    printf("HELLO FROM CALLBACK\n");
}


int main() {
    int speed = 2;

    xpix_init(FPS, "Test", 800, 600, pink);

    int x = 400;
    int y = 300;

    int dx = 1;
    int dy = 1;

    profiler_init(0);

    XPix_ID sub = xpix_createSubwindow(0, 300, 300, 200, 200, 0xFFFF0000, 5, 0);
    XPix_ID btn = xpix_createDefaultButton(callback, sub, 0, 0, 100, 50);

    while(!xpix_shouldQuit()) {
       
        // clear window and process events
        profiler_begin("frame", PROFILER_MS * 17);
        xpix_beginFrame();

        dx = xpix_getMouseX() < x+50 ? -1 : 1;
        dy = xpix_getMouseY() < y+50 ? -1 : 1;

        x += dx * speed;
        y += dy * speed;

        if (xpix_isKeyPressed(XPIX_KEY_E)) {
            xpix_setBackground(sub, 0xFF0000FF);
        }

        xpix_drawRectangle(x, y, 100, 100, blue);
      
        // apply window changes
        profiler_end();
        xpix_endFrame();
    }
    
    profiler_destroy();
    xpix_shutdown();
    return 0;
}