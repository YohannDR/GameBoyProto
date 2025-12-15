#include "sprites_AI/pressure_plate.h"

#include "gb/oam.h"
#include "sprite.h"
#include "player.h"

enum PressurePlatePose {
    POSE_IDLE = 1,
    POSE_PRESSED
};

extern const struct AnimData sPressurePlate_Off[];

static u8 IsPlayerOnPressurePlate(void)
{
    if (gPlayerData.size == PLAYER_SIZE_SMALL)
        return FALSE;

    if (gPlayerData.y < gCurrentSprite.y + THREE_QUARTER_BLOCK_SIZE)
        return FALSE;

    if (gPlayerData.y > gCurrentSprite.y + BLOCK_SIZE)
        return FALSE;

    if (gPlayerData.x + BLOCK_SIZE < gCurrentSprite.x - BLOCK_SIZE * 2)
        return FALSE;

    if (gPlayerData.x > gCurrentSprite.x + BLOCK_SIZE)
        return FALSE;

    return TRUE;
}

void PressurePlate(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.x += BLOCK_SIZE;
        gCurrentSprite.y -= BLOCK_SIZE;

        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(QUARTER_BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);

        gCurrentSprite.animPointer = sPressurePlate_Off;
        gCurrentSprite.pose = POSE_IDLE;
    }

    if (gCurrentSprite.pose == POSE_IDLE)
    {
        if (IsPlayerOnPressurePlate())
        {
            gCurrentSprite.status |= SPRITE_STATUS_NOT_DRAWN;
            gCurrentSprite.pose = POSE_PRESSED;
        }
    }
    else if (gCurrentSprite.pose == POSE_PRESSED)
    {
        if (!IsPlayerOnPressurePlate())
        {
            gCurrentSprite.status &= ~SPRITE_STATUS_NOT_DRAWN;
            gCurrentSprite.pose = POSE_IDLE;
        }
    }
}

const u8 sPressurePlateGraphics[] = {
    1,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
};

static const u8 sPressurePlate_Off_Frame0[OAM_DATA_SIZE(2)] = {
    2,
    OAM_POS(0), OAM_POS(0), 0, 0,
    OAM_POS(0), OAM_POS(-8), 0, 0,
};

const struct AnimData sPressurePlate_Off[] = {
    [0] = {
        .oamPointer = sPressurePlate_Off_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
