#include "player/player_movement.h"

#include "player/player.h"
#include "input.h"
#include "bg_clip.h"

#define LADDER_SPEED (PIXEL_SIZE)

struct PlayerPhysics gPlayerPhysics;
struct PlayerMovement gPlayerMovement;

void PlayerInitPhysics(void)
{
    gPlayerPhysics.xAcceleration = PIXEL_TO_SUB_PIXEL(.5);
    gPlayerPhysics.xDeceleration = PIXEL_TO_SUB_PIXEL(.75);
    gPlayerPhysics.xVelocityCap = PIXEL_TO_SUB_PIXEL(6);
    gPlayerPhysics.yVelocityCap = PIXEL_TO_SUB_PIXEL(8);
    gPlayerPhysics.gravityUpwards = PIXEL_TO_SUB_PIXEL(.75);
    gPlayerPhysics.gravityDownwards = PIXEL_TO_SUB_PIXEL(1);
    gPlayerPhysics.jumpingVelocity = PIXEL_TO_SUB_PIXEL(-14);

    gPlayerMovement.gravity = gPlayerPhysics.gravityDownwards;
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

    if (gButtonInput & KEY_RIGHT)
    {
        gPlayerMovement.xVelocity += gPlayerPhysics.xAcceleration;
        gPlayerMovement.direction = KEY_RIGHT;

        if (gPlayerMovement.xVelocity > gPlayerPhysics.xVelocityCap)
            gPlayerMovement.xVelocity = gPlayerPhysics.xVelocityCap;
    }

    // Apply deceleration
    if (!(gButtonInput & (KEY_RIGHT | KEY_LEFT)))
    {
        if (gPlayerMovement.xVelocity > 0)
        {
            gPlayerMovement.xVelocity -= gPlayerPhysics.xDeceleration;
            if (gPlayerMovement.xVelocity < 0)
                gPlayerMovement.xVelocity = 0;
        }
        else if (gPlayerMovement.xVelocity < 0)
        {
            gPlayerMovement.xVelocity += gPlayerPhysics.xDeceleration;
            if (gPlayerMovement.xVelocity > 0)
                gPlayerMovement.xVelocity = 0;
        }
    }
}

void PlayerVerticalMovement(void)
{
    if (gPlayerMovement.grounded && gChangedInput & JUMP_BUTTON)
        PlayerInitJump();

    gPlayerMovement.yVelocity += gPlayerMovement.gravity;
    if (gPlayerMovement.yVelocity > 0)
    {
        if (gPlayerMovement.yVelocity > gPlayerPhysics.yVelocityCap)
            gPlayerMovement.yVelocity = gPlayerPhysics.yVelocityCap;
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
        PlayerInitJump();
}

void PlayerInitJump(void)
{
    gPlayerMovement.yVelocity = gPlayerPhysics.jumpingVelocity;
    gPlayerMovement.gravity = gPlayerPhysics.gravityUpwards;
    gPlayerMovement.grounded = FALSE;
    PlayerSetPose(PLAYER_POSE_JUMPING);
}

void PlayerJumpMovement(void)
{
    // If the button has been released, or the jump apex was reached
    if (!(gButtonInput & JUMP_BUTTON) || gPlayerMovement.yVelocity > 0)
    {
        // Switch to downward gravity
        gPlayerMovement.gravity = gPlayerPhysics.gravityDownwards;
        // And cancel any upwards movement we had somewhat smoothly
        if (gPlayerMovement.yVelocity < 0)
            gPlayerMovement.yVelocity /= 2;
    }
}

void PlayerApplyMovement(void)
{
    gPlayerData.x += SUB_PIXEL_TO_PIXEL(gPlayerMovement.xVelocity);
    gPlayerData.y += SUB_PIXEL_TO_PIXEL(gPlayerMovement.yVelocity);
}
