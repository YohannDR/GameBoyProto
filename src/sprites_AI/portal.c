#include "sprites_AI/portal.h"

#include "gb/oam.h"
#include "input.h"
#include "sprite.h"
#include "player/player.h"

extern const struct AnimData sPortalAnim_Idle[];

static u8 IsPlayerUsingPortal(void)
{
    if (gCurrentSprite.part == gPlayerData.size)
        return FALSE;

    if (!(gChangedInput & KEY_UP))
       return FALSE;

    if (gPlayerData.y < gCurrentSprite.y - BLOCK_SIZE * 3)
        return FALSE;

    if (gPlayerData.y - PLAYER_HEIGHT > gCurrentSprite.y)
        return FALSE;

    if (gPlayerData.x > gCurrentSprite.x + BLOCK_SIZE * 2 - QUARTER_BLOCK_SIZE)
        return FALSE;

    if (gPlayerData.x + PLAYER_WIDTH < gCurrentSprite.x + QUARTER_BLOCK_SIZE)
        return FALSE;

    return TRUE;
}

void Portal(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.part = PLAYER_SIZE_SMALL;

        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE * 3);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);

        gCurrentSprite.pose = 1;
        gCurrentSprite.animPointer = sPortalAnim_Idle;
    }

    gCurrentSprite.status &= ~SPRITE_STATUS_NOT_DRAWN;
    if (IsPlayerUsingPortal())
    {
        // TODO Initiate size change
        gCurrentSprite.status |= SPRITE_STATUS_NOT_DRAWN;
    }
}

const u8 sPortalGraphics[] = {
    2,

    0x00, 0x00, 0x0F, 0x0F, 0x38, 0x3F, 0x27, 0x38, 0x68, 0x77, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F,
    0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F, 0x50, 0x6F,
};

static const u8 sPortalAnim_Idle_Frame0[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-24), OAM_POS(0), 0, 0,
    OAM_POS(-24), OAM_POS(8), 0, 32,
    OAM_POS(-16), OAM_POS(0), 1, 0,
    OAM_POS(-16), OAM_POS(8), 1, 32,
    OAM_POS(-8), OAM_POS(0), 0, 64,
    OAM_POS(-8), OAM_POS(8), 0, 96,
};

const struct AnimData sPortalAnim_Idle[] = {
    [0] = {
        .oamPointer = sPortalAnim_Idle_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
