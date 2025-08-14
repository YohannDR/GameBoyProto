#include "sprites_AI/water_drop.h"

#include "gb/oam.h"

#include "bg_clip.h"
#include "sprite.h"

extern const struct AnimData sWaterDropAnim_Idle[];

enum WaterDropPose {
    POSE_FALLING = 1,
    POSE_FLYING,
    POSE_EXPLODING
};

#define WATER_DROP_FALLING_SPEED (QUARTER_BLOCK_SIZE)
#define WATER_DROP_FLYING_SPEED (QUARTER_BLOCK_SIZE)

#define WATER_DROP_FLYING_LIFETIME (BLOCK_SIZE * 5)

static void WaterDropExplodingInit(void)
{
    gCurrentSprite.pose = POSE_EXPLODING;

    // gCurrentSprite.animPointer = sWaterDropAnim_Exploding;
    gCurrentSprite.currentAnimFrame = 0;
    gCurrentSprite.animTimer = 0;
}

void WaterDrop(void)
{
    if (gCurrentSprite.pose == 0)
    {
        if (gCurrentSprite.part == WATER_DROP_FALLING)
            gCurrentSprite.pose = POSE_FALLING;
        else
            gCurrentSprite.pose = POSE_FLYING;

        gCurrentSprite.animPointer = sWaterDropAnim_Idle;
    }
    else if (gCurrentSprite.pose == POSE_FALLING)
    {
        gCurrentSprite.y += WATER_DROP_FALLING_SPEED;
    }
    else if (gCurrentSprite.pose == POSE_FLYING)
    {
        if (gCurrentSprite.part == WATER_DROP_FLYING_LEFT)
            gCurrentSprite.x -= WATER_DROP_FLYING_SPEED;
        else
            gCurrentSprite.x += WATER_DROP_FLYING_SPEED;

        gCurrentSprite.work1++;

        if (gCurrentSprite.work1 == WATER_DROP_FLYING_LIFETIME / WATER_DROP_FLYING_SPEED)
        {
            WaterDropExplodingInit();
            // We can exit out early, no need to check for collision
            return;
        }
    }
    else if (gCurrentSprite.pose == POSE_EXPLODING)
    {
        if (gCurrentSprite.status & SPRITE_STATUS_ANIM_ENDED)
            gCurrentSprite.status &= ~SPRITE_STATUS_EXISTS;

        // We can exit out early, no need to check for collision
        return;
    }

    // Check to see if the water is colliding with anything
    if (GET_CLIPDATA_SOLIDITY(gCurrentSprite.x, gCurrentSprite.y) == COLLISION_SOLID)
        WaterDropExplodingInit();
}

const u8 sWaterDropGraphics[] = {
    1,

    0x00, 0x18, 0x00, 0x24, 0x00, 0x42, 0x00, 0x81, 0x00, 0x81, 0x00, 0x42, 0x00, 0x24, 0x00, 0x18,
};

static const u8 sWaterDropAnim_Idle_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-8), OAM_POS(0), 0, 0,
};

const struct AnimData sWaterDropAnim_Idle[] = {
    [0] = {
        .oamPointer = sWaterDropAnim_Idle_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
