#include "sprites_AI/ball.h"

#include "gb/display.h"

#include "types.h"
#include "sprite.h"
#include "input.h"

static const u8 sBallAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 0x8F, 0
};

static const struct AnimData sBallAnim[] = {
    [0] = {
        .oamPointer = sBallAnim_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

enum BallPose {
    WAITING_TO_START = 1,
    MOVING
};

#define MOVING_RIGHT (1 << 0)
#define MOVING_DOWN  (1 << 1)

void Ball(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sBallAnim;
        gCurrentSprite.pose = WAITING_TO_START;
    }

    if (gCurrentSprite.pose == WAITING_TO_START)
    {
        if (gChangedInput & KEY_A)
            gCurrentSprite.pose = MOVING;

        return;
    }

    if (gCurrentSprite.work1 & MOVING_RIGHT)
    {
        gCurrentSprite.x++;

        if (gCurrentSprite.x == SCREEN_SIZE_X - 7u)
            gCurrentSprite.work1 &= ~MOVING_RIGHT;
    }
    else
    {
        gCurrentSprite.x--;

        if (gCurrentSprite.x == 16u)
            gCurrentSprite.work1 |= MOVING_RIGHT;
    }

    if (gCurrentSprite.work1 & MOVING_DOWN)
    {
        gCurrentSprite.y++;

        if (gCurrentSprite.y == SCREEN_SIZE_Y + 16u)
            gCurrentSprite.work1 &= ~MOVING_DOWN;
    }
    else
    {
        gCurrentSprite.y--;

        if (gCurrentSprite.y == 24u)
            gCurrentSprite.work1 |= MOVING_DOWN;
    }
}
