#ifndef SCROLL_H
#define SCROLL_H

#include "macros.h"

#define SCROLL_X_ANCHOR (SCREEN_SIZE_X_SUB_PIXEL / 2)
#define SCROLL_Y_ANCHOR (SCREEN_SIZE_Y_SUB_PIXEL / 2)

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
u16 GetCameraTargetX(void);
u16 GetCameraTargetY(void);

#endif /* SCROLL_H */
