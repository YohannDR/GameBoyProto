#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include "types.h"

enum MovingPlatformType {
    MOVING_PLATFORM_LEFT,
    MOVING_PLATFORM_RIGHT,
    MOVING_PLATFORM_UP,
    MOVING_PLATFORM_DOWN,
};

extern const u8 sMovingPlatformGraphics[];

void MovingPlatform(void);

#endif /* MOVING_PLATFORM_H */
