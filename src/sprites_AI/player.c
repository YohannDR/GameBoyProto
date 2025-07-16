#include "sprites_AI/player.h"

#include "input.h"
#include "bg.h"
#include "gb/oam.h"
#include "sprite.h"

static const u8 sPlayerOam_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(0), 0x19, 0
};

static const struct AnimData sPlayerAnim[] = {
    [0] = {
        .oamPointer = sPlayerOam_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

void Player(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sPlayerAnim;
        gCurrentSprite.pose = 1;
    }

    if (gButtonInput & KEY_LEFT && gBackgroundInfo.x != 0)
    {
        gBackgroundInfo.x--;
        gCurrentSprite.x -= PIXEL_SIZE;
    }

    if (gButtonInput & KEY_RIGHT && gBackgroundInfo.blockX + 0x14 != gTilemap.width)
    {
        gBackgroundInfo.x++;
        gCurrentSprite.x += PIXEL_SIZE;
    }

    /*
    if (gButtonInput & KEY_UP && gBackgroundY != 0)
        gBackgroundY--;

    if (gButtonInput & KEY_DOWN)
        gBackgroundY++;
    */
}
