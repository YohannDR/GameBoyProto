#include "sprites_AI/ship.h"

#include "gb/oam.h"

#include "sprite.h"

#define MOVING_SPEED (PIXEL_SIZE)

void Ship(void)
{
    // TODO proper initialization (setting animation, draw distance, X flipping if going right and pose 1)

    // TODO proper screen looping? This loops via underflow/overflow which is a bit weird
    if (gCurrentSprite.part == SHIP_LEFT)
        gCurrentSprite.x -= MOVING_SPEED;
    else if (gCurrentSprite.part == SHIP_RIGHT)
        gCurrentSprite.x += MOVING_SPEED;
}

const u8 sShipGraphics[] = {
    1,

    0x0B, 0x0B, 0x1D, 0x1F, 0x2F, 0x2F, 0x7F, 0x7D, 0xBD, 0xC7, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00,
};
