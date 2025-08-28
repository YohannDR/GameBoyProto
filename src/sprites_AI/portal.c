#include "sprites_AI/portal.h"

#include "gb/oam.h"

#include "sprite.h"
#include "player.h"
#include "input.h"
#include "time.h"

extern const struct AnimData sPortalAnim_Idle[];

enum PortalPose {
    POSE_IDLE = 1,
    POSE_WARPING
};

void PortalCheckCollision(void)
{
    if (!(gChangedInput & KEY_UP))
        return;

    if (!(gCurrentSprite.status & SPRITE_STATUS_ON_SCREEN))
        return;

    if (gPlayerData.x + PLAYER_WIDTH < gCurrentSprite.x)
        return;

    if (gPlayerData.x > gCurrentSprite.x + BLOCK_SIZE * 2)
        return;

    if (gPlayerData.y < gCurrentSprite.y)
        return;

    if (gPlayerData.y - PLAYER_HEIGHT > gCurrentSprite.y + BLOCK_SIZE * 4)
        return;

    gCurrentSprite.pose = POSE_WARPING;
    gCurrentSprite.work1 = CONVERT_SECONDS(4.f);
    StartPortalTransition();
}

void Portal(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sPortalAnim_Idle;
        gCurrentSprite.pose = POSE_IDLE;

        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE * 2);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE * 3);
    }
    
    if (gCurrentSprite.pose == POSE_IDLE)
        PortalCheckCollision();

    if (gCurrentSprite.pose == POSE_WARPING)
    {
        gCurrentSprite.work1 -= DELTA_TIME;

        if (gCurrentSprite.work1 == 0)
            gCurrentSprite.pose = POSE_IDLE;
    }
}

static const u8 sPortalAnim_Idle_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(0), 0, 0,
};

const struct AnimData sPortalAnim_Idle[] = {
    [0] = {
        .oamPointer = sPortalAnim_Idle_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sPortalGraphics[] = {
    4,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x00, 0x08, 0x07, 0x10, 0x0C, 0x0C, 0x13,
    0x2B, 0x14, 0x22, 0x14, 0x03, 0x14, 0x1C, 0x03, 0x11, 0x0C, 0x0C, 0x00, 0x04, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0xC0, 0xA0, 0x40, 0x44, 0xA0,
    0x50, 0xA0, 0xD0, 0x20, 0x90, 0x60, 0xD0, 0x20, 0x20, 0xC0, 0x20, 0xC0, 0x40, 0x80, 0x00, 0x00,
};
