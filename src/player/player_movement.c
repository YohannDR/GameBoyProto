#include "player/player_movement.h"

#include "player/player.h"
#include "input.h"
#include "bg_clip.h"

#define LADDER_SPEED (PIXEL_SIZE)

struct PlayerPhysics gPlayerPhysics;
struct PlayerMovement gPlayerMovement;

void PlayerInitPhysics(void)
{
    gPlayerPhysics.xAcceleration = 1;
    gPlayerPhysics.xVelocityCap = 4;
    gPlayerPhysics.yVelocityCap = 8;
    gPlayerPhysics.gravityUpwards = 1;
    gPlayerPhysics.gravityDownwards = 1;
    gPlayerPhysics.jumpingVelocity = -12;
}

void PlayerHorizontalMovement(void)
{
    if (gButtonInput & KEY_LEFT)
    {
        gPlayerMovement.xVelocity -= gPlayerPhysics.xAcceleration;
        gPlayerMovement.direction = KEY_LEFT;

        if (gPlayerMovement.xVelocity < -gPlayerPhysics.xVelocityCap)
            gPlayerMovement.xVelocity = -gPlayerPhysics.xVelocityCap;
    }
    else
    {
        if (gPlayerMovement.xVelocity < 0)
            gPlayerMovement.xVelocity = 0;
    }

    if (gButtonInput & KEY_RIGHT)
    {
        gPlayerMovement.xVelocity += gPlayerPhysics.xAcceleration;
        gPlayerMovement.direction = KEY_RIGHT;

        if (gPlayerMovement.xVelocity > gPlayerPhysics.xVelocityCap)
            gPlayerMovement.xVelocity = gPlayerPhysics.xVelocityCap;
    }
    else
    {
        if (gPlayerMovement.xVelocity > 0)
            gPlayerMovement.xVelocity = 0;
    }
}

void PlayerVerticalMovement(void)
{
    if (gPlayerMovement.grounded && gChangedInput & JUMP_BUTTON)
    {
        gPlayerMovement.yVelocity = gPlayerPhysics.jumpingVelocity;
        gPlayerMovement.grounded = FALSE;
    }

    if (gPlayerMovement.yVelocity > 0)
    {
        gPlayerMovement.yVelocity += gPlayerPhysics.gravityDownwards;
        if (gPlayerMovement.yVelocity > gPlayerPhysics.yVelocityCap)
            gPlayerMovement.yVelocity = gPlayerPhysics.yVelocityCap;
    }
    else
    {
        gPlayerMovement.yVelocity += gPlayerPhysics.gravityUpwards;
    }
}

void PlayerLadderMovement(void)
{
    if (gButtonInput & KEY_DOWN)
    {
        gPlayerData.y += LADDER_SPEED * DELTA_TIME;

        if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y) != CLIP_BEHAVIOR_LADDER)
            PlayerSetPose(PLAYER_POSE_IDLE);
    }
    else if (gButtonInput & KEY_UP)
    {
        gPlayerData.y -= LADDER_SPEED * DELTA_TIME;

        if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y - PIXEL_SIZE) != CLIP_BEHAVIOR_LADDER)
            PlayerSetPose(PLAYER_POSE_IDLE);
    }

    if (gChangedInput & KEY_LEFT)
        gPlayerMovement.direction = KEY_LEFT;
    else if (gChangedInput & KEY_RIGHT)
        gPlayerMovement.direction = KEY_RIGHT;

    if (gChangedInput & JUMP_BUTTON)
    {
        gPlayerMovement.yVelocity = gPlayerPhysics.jumpingVelocity;
        gPlayerMovement.grounded = FALSE;
        PlayerSetPose(PLAYER_POSE_IDLE);
    }
}

void PlayerApplyMovement(void)
{
    gPlayerData.x += gPlayerMovement.xVelocity;
    gPlayerData.y += gPlayerMovement.yVelocity;
}
