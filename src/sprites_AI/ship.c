#pragma bank 2

#include "sprites_AI/ship.h"

#include "gb/oam.h"

#include "bg.h"
#include "macros.h"
#include "sprite.h"

#define MOVING_SPEED (PIXEL_SIZE)

extern const struct AnimData sShipAnim_Idle[];

void Ship(void)
{
    // proper initialization (setting animation, draw distance, X flipping if going right and pose 1)
    if (gCurrentSprite.pose == 0)
    {   
        gCurrentSprite.animPointer = sShipAnim_Idle;
        gCurrentSprite.pose = 1;

        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);

        if (gCurrentSprite.part == SHIP_RIGHT)
            gCurrentSprite.properties |= SPRITE_PROPERTY_X_FLIP;
    }

    // Screen looping 
    if (gCurrentSprite.part == SHIP_LEFT)
    {
        if (gCurrentSprite.x < MOVING_SPEED)
            gCurrentSprite.x = BLOCK_TO_SUB_PIXEL(gTilemap.width);
        else
            gCurrentSprite.x -= MOVING_SPEED;
    }
    else if (gCurrentSprite.part == SHIP_RIGHT)
    {
        if (gCurrentSprite.x > BLOCK_TO_SUB_PIXEL(gTilemap.width))
            gCurrentSprite.x = 0;
        else
            gCurrentSprite.x += MOVING_SPEED;
    }
}

const u8 sShipGraphics[] = {
    1,

    0x0B, 0x0B, 0x1D, 0x1F, 0x2F, 0x2F, 0x7F, 0x7D, 0xBD, 0xC7, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00,
};

static const u8 sShipAnim_Idle_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-8), OAM_POS(0), 0, 0,
};

const struct AnimData sShipAnim_Idle[] = {
    [0] = {
        .oamPointer = sShipAnim_Idle_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
