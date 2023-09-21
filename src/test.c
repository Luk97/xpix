#include "xpix.h"

#include <unistd.h>
#include <stdio.h>

#define pink 0xFFFFAAAA
#define blue 0xFFAAAAFF
#define black 0xFF000000
#define FPS 60


int main() {
    int speed = 100;
    unsigned int radius = 50;

    xpix_init(FPS, "Test", 800, 600, pink);

    XPix_Circle circle = {
        .x = 400,
        .y = 300,
        .radius = radius,
    };

    int dx = 1;
    int dy = 1;

    while(!xpix_shouldQuit()) {
       
        // clear window and process events
        xpix_startFrame();

        if (xpix_isKeyPressed(XPIX_KEY_E)) {
            speed -= 10;
        }
        if (xpix_isKeyPressed(XPIX_KEY_R)) {
            speed += 10;
        }

        dx = xpix_getMouseX() < circle.x+radius ? -1 : 1;
        dy = xpix_getMouseY() < circle.y+radius ? -1 : 1;
        
        circle.x += dx * speed * xpix_getDeltaTime();
        circle.y += dy * speed * xpix_getDeltaTime();
        

        printf("FPS: %d\n", xpix_getRealFps());
        printf("delta: %f\n", xpix_getDeltaTime());
        
        xpix_drawCircleCirc(&circle, black);
      
        // apply window changes
        xpix_endFrame();
    }
    
    xpix_shutdown();
    return 0;
}