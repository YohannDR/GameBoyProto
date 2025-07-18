#include "player.h"

struct PlayerPhysics gPlayerPhysics;
struct PlayerMovement gPlayerMovement;

static void PlayerInitPhysics(void)
{
    gPlayerPhysics.xAcceleration = ONE_SUB_PIXEL;
    gPlayerPhysics.xVelocityCap = QUARTER_BLOCK_SIZE;
    gPlayerPhysics.yVelocityCap = QUARTER_BLOCK_SIZE;

    gPlayerPhysics.gravityUpwards = ONE_SUB_PIXEL;
    gPlayerPhysics.gravityDownwards = PIXEL_SIZE / 2;

    gPlayerPhysics.jumpingVelocity = -HALF_BLOCK_SIZE;
}

void PlayerInit(void)
{
    PlayerInitPhysics();
}
