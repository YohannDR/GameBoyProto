#include "sprites_AI/player.h"

#include "gb/oam.h"

#include "input.h"

static const u8 sPlayerAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 25, 0,
};

static const u8 sPlayerAnim_Frame1[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 25, OAM_ATTR_X_FLIP
};

static const struct AnimData sPlayerAnim[] = {
    [0] = {
        .oamPointer = sPlayerAnim_Frame0,
        .duration = 30
    },
    [1] = {
        .oamPointer = sPlayerAnim_Frame1,
        .duration = 30
    },
    [2] = SPRITE_ANIM_TERMINATOR
};

void Player(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sPlayerAnim;
        gCurrentSprite.pose = 1;
    }

    if (gButtonInput & KEY_LEFT)
        gCurrentSprite.x--;
    else if (gButtonInput & KEY_RIGHT)
        gCurrentSprite.x++;

    if (gButtonInput & KEY_UP)
        gCurrentSprite.y--;
    else if (gButtonInput & KEY_DOWN)
        gCurrentSprite.y++;

    if (gChangedInput & KEY_A)
        gCurrentSprite.properties ^= SPRITE_PROPERTY_X_FLIP;
    if (gChangedInput & KEY_B)
        gCurrentSprite.properties ^= SPRITE_PROPERTY_Y_FLIP;
}
