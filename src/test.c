#include "xpix.h"

#include <unistd.h>
#include <stdio.h>

#define PROFILER_IMPLEMENTATION
#include "../thirdparty/profiler.h"

#define pink 0xFFFFAAAA
#define blue 0xFFAAAAFF
#define black 0xFF000000
#define red 0xFFFF4444
#define FPS 60

void callback() {
    printf("HELLO FROM CALLBACK\n");
}


int main() {
    profiler_init(PROFILER_LOG_INFO);

    int window_width = 800;
    int window_height = 600;

    xpix_init(FPS, "Test", window_width, window_height, pink);

 


    while(!xpix_shouldQuit()) {
       
        // clear window and process events
        //profiler_begin("frame", 0);
        xpix_beginFrame();

        XPix_Point p = xpix_getMouseMovement();
        printf("delta x: %d, delta y: %d\n", p.x, p.y);

      
        //profiler_end();
        xpix_endFrame();
    }
    
    profiler_destroy();
    xpix_shutdown();
    return 0;
}