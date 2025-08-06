#ifndef SCROLL_H
#define SCROLL_H

#include "macros.h"

struct Camera {
    u16 x;
    u16 y;

    s16 xVelocity;
    s16 yVelocity;

    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
};

extern struct Camera gCamera;

void SetCameraPosition(u16 x, u16 y);
void ScrollUpdate(void);

#endif /* SCROLL_H */
