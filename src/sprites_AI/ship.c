#include "sprites_AI/ship.h"

#include "gb/oam.h"

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

    // TODO proper screen looping? This loops via underflow/overflow which is a bit weird
    // When moving left, check if x is below the speed (we can't check if x < 0 because it's unsigned, so it's UB), if yes loop to tilemap width (don't forget to convert from block to sub pixel)
    // When moving right, check if x is above the tilemap width (don't forget to convert from block to sub pixel), if yes loop to 0
    // If you want an example of this kind of logic, check inventory.c, lines 223 to 237
    if (gCurrentSprite.part == SHIP_LEFT)
        gCurrentSprite.x -= MOVING_SPEED;
    else if (gCurrentSprite.part == SHIP_RIGHT)
        gCurrentSprite.x += MOVING_SPEED;
}

const u8 sShipGraphics[] = {
    1,

    0x0B, 0x0B, 0x1D, 0x1F, 0x2F, 0x2F, 0x7F, 0x7D, 0xBD, 0xC7, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00,
};
