#include "sprites_AI/moving_platform.h"

#include "gb/oam.h"

#include "sprite.h"

enum MovingPlatformPose {
    POSE_IDLE = 1,
    POSE_MOVING
};

#define MOVING_SPEED (PIXEL_SIZE)
#define MOVING_RANGE (BLOCK_SIZE * 3)

extern const struct AnimData sMovingPlatformAnim_Idle[];

void MovingPlatform(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.pose = POSE_IDLE;
        gCurrentSprite.animPointer = sMovingPlatformAnim_Idle;

        gCurrentSprite.work1 = 60;

        gCurrentSprite.drawDistanceLeft = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE + HALF_BLOCK_SIZE);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE + HALF_BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);
        return;
    }

    if (gCurrentSprite.pose == POSE_IDLE)
    {
        // This is testing code, normally we would wait for a signal to move
        gCurrentSprite.work1--;
        if (gCurrentSprite.work1 == 0)
        {
            gCurrentSprite.work1 = 0;
            gCurrentSprite.pose = POSE_MOVING;
        }
        return;
    }

    if (gCurrentSprite.pose == POSE_MOVING)
    {
        if (gCurrentSprite.part == MOVING_PLATFORM_LEFT)
            gCurrentSprite.x -= MOVING_SPEED;
        else if (gCurrentSprite.part == MOVING_PLATFORM_RIGHT)
            gCurrentSprite.x += MOVING_SPEED;
        else if (gCurrentSprite.part == MOVING_PLATFORM_UP)
            gCurrentSprite.y -= MOVING_SPEED;
        else if (gCurrentSprite.part == MOVING_PLATFORM_DOWN)
            gCurrentSprite.y += MOVING_SPEED;

        gCurrentSprite.work1++;

        if (gCurrentSprite.work1 == MOVING_RANGE / MOVING_SPEED)
        {
            gCurrentSprite.pose = POSE_IDLE;

            if (gCurrentSprite.part == MOVING_PLATFORM_LEFT)
                gCurrentSprite.part = MOVING_PLATFORM_RIGHT;
            else if (gCurrentSprite.part == MOVING_PLATFORM_RIGHT)
                gCurrentSprite.part = MOVING_PLATFORM_LEFT;
            else if (gCurrentSprite.part == MOVING_PLATFORM_UP)
                gCurrentSprite.part = MOVING_PLATFORM_DOWN;
            else if (gCurrentSprite.part == MOVING_PLATFORM_DOWN)
                gCurrentSprite.part = MOVING_PLATFORM_UP;
        }
    }
}

const u8 sMovingPlatformGraphics[] = {
    2,

    0xFF, 0xFF, 0x80, 0xFF, 0xBF, 0xC0, 0xBF, 0xC0, 0xBF, 0xC0, 0xBF, 0xC0, 0x80, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
};

static const u8 sMovingPlatformAnim_Idle_Frame0[OAM_DATA_SIZE(3)] = {
    3,
    OAM_POS(-8), OAM_POS(-4), 1, 0,
    OAM_POS(-8), OAM_POS(4), 0, 32,
    OAM_POS(-8), OAM_POS(-12), 0, 0,
};

const struct AnimData sMovingPlatformAnim_Idle[] = {
    [0] = {
        .oamPointer = sMovingPlatformAnim_Idle_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
