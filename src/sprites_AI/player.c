#include "sprites_AI/player.h"

#include "input.h"

static const u8 sPlayerAnim_Frame0[OAM_DATA_SIZE(2)] = {
    2,
    0, 0, 3, 0,
    0, 8, 4, 0
};

static const u8 sPlayerAnim_Frame1[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 2, 0
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

void Player(struct Sprite* sprite)
{
    if (sprite->pose == 0)
    {
        sprite->animPointer = sPlayerAnim;
        sprite->pose = 1;
    }

    if (gButtonInput & KEY_LEFT)
        sprite->x--;
    else if (gButtonInput & KEY_RIGHT)
        sprite->x++;

    if (gButtonInput & KEY_UP)
        sprite->y--;
    else if (gButtonInput & KEY_DOWN)
        sprite->y++;
}
