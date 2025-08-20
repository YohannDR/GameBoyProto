#include "sprites_AI/collectible.h"

#include "gb/oam.h"

#include "inventory.h"
#include "player.h"
#include "sprite.h"

extern const struct AnimData sCollectibleAnim_Torch[];

static void SetCollectibleAnim(void)
{
    if (gCurrentSprite.part == ITEM_TORCH)
        gCurrentSprite.animPointer = sCollectibleAnim_Torch;
}

static void HandleFloatingMovement(void)
{
    // Increment timer
    gCurrentSprite.work1 += DELTA_TIME;

    if (gCurrentSprite.work1 == CONVERT_SECONDS(.4f))
    {
        // Move in specified direction
        if (gCurrentSprite.work3)
            gCurrentSprite.y += PIXEL_SIZE;
        else
            gCurrentSprite.y -= PIXEL_SIZE;

        // Reset timer
        gCurrentSprite.work1 = 0;
        // Increase amount of times we moved
        gCurrentSprite.work2++;

        // Check if we moved enough to change direction
        if (gCurrentSprite.work2 == 2)
        {
            // Flip direction
            gCurrentSprite.work3 ^= TRUE;
            // And reset movement counter
            gCurrentSprite.work2 = 0;
        }
    }
}

static void CheckPlayerCollision(void)
{
    if (!(gCurrentSprite.status & SPRITE_STATUS_ON_SCREEN))
        return;

    // Compute collision with a smaller hitbox
    if (gPlayerData.x + PLAYER_WIDTH / 3 * 2 < gCurrentSprite.x)
        return;

    if (gPlayerData.x + PLAYER_WIDTH / 3 * 1 > gCurrentSprite.x + BLOCK_SIZE)
        return;

    if (gPlayerData.y - PLAYER_HEIGHT / 3 * 1 < gCurrentSprite.y - BLOCK_SIZE)
        return;

    if (gPlayerData.y - PLAYER_HEIGHT / 3 * 2 > gCurrentSprite.y)
        return;

    // Give the item
    RegisterItem(gCurrentSprite.part);
    // And kill this sprite
    gCurrentSprite.status = 0;
}

void Collectible(void)
{
    if (gCurrentSprite.pose == 0)
    {
        if (HasItem(gCurrentSprite.part))
        {
            // We already have the item, we can kill the sprite
            gCurrentSprite.status = 0;
            return;
        }

        gCurrentSprite.pose = 1;
        SetCollectibleAnim();
    }

    HandleFloatingMovement();
    CheckPlayerCollision();
}

static const u8 sCollectibleAnim_Torch_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-8), OAM_POS(0), 0, 0,
};

const struct AnimData sCollectibleAnim_Torch[] = {
    [0] = {
        .oamPointer = sCollectibleAnim_Torch_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sCollectibleGraphics[] = {
    1,

    0x04, 0x04, 0x0A, 0x0E, 0x0A, 0x0E, 0x0C, 0x14, 0x10, 0x28, 0x20, 0x50, 0x40, 0xA0, 0x00, 0xC0,
};
