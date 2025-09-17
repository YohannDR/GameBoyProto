#pragma bank 2

#include "sprites_AI/fire_spawner.h"

#include "fire.h"
#include "sprite.h"

void FireSpawner(void)
{
    u8 direction;
    u8 length;

    // The fire spawner sprite is used to create a fire cluster at a specified location, with a specified length.
    // As such, it needs to take 2 parameters, the direction and the length.
    // We can represent both of these on a single byte using the "part", by using one nibble to express the direction as bit flags,
    // and another nibble to express the length.
    //
    // 1111             1111
    // ^                ^
    // upper nibble     lower nibble
    // direction flags  length value
    //
    // So for example, a value of 0x85 would represent a fire cluster going right, with a length of 5
    // 0x85 = 0b10000101 : 1000              0101
    //                     ^                 ^
    //                     1 << 7, so right  5
    // The direction flags use the same order as the internal fire status flag, so :
    // up    = 1 << 4 (0x10, 16)
    // down  = 1 << 5 (0x20, 32)
    // left  = 1 << 6 (0x40, 64)
    // right = 1 << 7 (0x80, 128)

    direction = GET_UPPER_NIBBLE(gCurrentSprite.part);
    length = GET_LOWER_NIBBLE(gCurrentSprite.part);

    SpawnCluster(gCurrentSprite.x, gCurrentSprite.y - BLOCK_SIZE, direction, length);

    // The sprite has done its job, we can kill it immediatly
    gCurrentSprite.status = 0;
}
