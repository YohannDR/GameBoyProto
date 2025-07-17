#include "player.h"

struct PlayerMovement gPlayerMovement;

static void PlayerInitMovement(void)
{
    gPlayerMovement.xVelocity = 0;
    gPlayerMovement.yVelocity = 0;

    gPlayerMovement.xAcceleration = ONE_SUB_PIXEL;
    gPlayerMovement.xVelocityCap = QUARTER_BLOCK_SIZE;
    gPlayerMovement.yVelocityCap = QUARTER_BLOCK_SIZE;

    gPlayerMovement.gravityUpwards = ONE_SUB_PIXEL;
    gPlayerMovement.gravityDownwards = PIXEL_SIZE / 2;
}

void PlayerInit(void)
{
    PlayerInitMovement();
}
