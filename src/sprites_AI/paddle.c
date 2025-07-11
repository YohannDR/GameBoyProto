#include "sprites_AI/paddle.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "input.h"

static const u8 sPaddleAnim_Frame0[OAM_DATA_SIZE(3)] = {
    3,
    OAM_POS(0), OAM_POS(-8), 0x8C, 0,
    OAM_POS(0), OAM_POS(0),  0x8D, 0,
    OAM_POS(0), OAM_POS(8),  0x8E, 0,
};

static const struct AnimData sPaddleAnim[] = {
    [0] = {
        .oamPointer = sPaddleAnim_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

enum PaddlePose {
    WAITING_TO_START = 1,
    MOVING
};

#define PADDLE_SPEED 2

void Paddle(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sPaddleAnim;
        gCurrentSprite.pose = WAITING_TO_START;
    }

    if (gCurrentSprite.pose == WAITING_TO_START)
    {
        if (gChangedInput & KEY_A)
            gCurrentSprite.pose = MOVING;

        return;
    }

    if (gButtonInput & KEY_LEFT)
    {
        if (gCurrentSprite.x >= 25)
            gCurrentSprite.x -= PADDLE_SPEED;
    }
    else if (gButtonInput & KEY_RIGHT)
    {
        if (gCurrentSprite.x <= SCREEN_SIZE_X - 17u)
            gCurrentSprite.x += PADDLE_SPEED;
    }
}
