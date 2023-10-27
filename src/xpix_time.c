#include "xpix_internal.h"

void xpix_setTargetFps(int fps) {
    // TODO: log invalid fps
    if (fps > 0) {
        CORE.Time.targetFps = fps;
    }
}

int xpix_getTargetFps() {
    return CORE.Time.targetFps;
}

int xpix_getFps() {
    return CORE.Time.realFps;
}

float xpix_getFrameTime() {
    return CORE.Time.frameTime;
}
