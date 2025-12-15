#include "player/player_collisions.h"

#include "bg_clip.h"
#include "player/player.h"
#include "player/player_movement.h"

struct HitboxData {
    s8 axisOffset;
    s8 pointsOffset[6];
};

static const struct HitboxData sHitboxLeft = {
    .axisOffset = -QUARTER_BLOCK_SIZE,
    .pointsOffset = {
        [0] = -(PLAYER_HEIGHT / 6 * 1 - PIXEL_SIZE),
        [1] = -(PLAYER_HEIGHT / 6 * 2 - PIXEL_SIZE),
        [2] = -(PLAYER_HEIGHT / 6 * 3 - PIXEL_SIZE),
        [3] = -(PLAYER_HEIGHT / 6 * 4 - PIXEL_SIZE),
        [4] = -(PLAYER_HEIGHT / 6 * 5 - PIXEL_SIZE),
        [5] = -(PLAYER_HEIGHT - PIXEL_SIZE)
    }
};

static const struct HitboxData sHitboxRight = {
    .axisOffset = PLAYER_WIDTH,
    .pointsOffset = {
        [0] = -(PLAYER_HEIGHT / 6 * 1 - PIXEL_SIZE),
        [1] = -(PLAYER_HEIGHT / 6 * 2 - PIXEL_SIZE),
        [2] = -(PLAYER_HEIGHT / 6 * 3 - PIXEL_SIZE),
        [3] = -(PLAYER_HEIGHT / 6 * 4 - PIXEL_SIZE),
        [4] = -(PLAYER_HEIGHT / 6 * 5 - PIXEL_SIZE),
        [5] = -(PLAYER_HEIGHT - PIXEL_SIZE)
    }
};

static const struct HitboxData sHitboxTop = {
    .axisOffset = -(PLAYER_HEIGHT + PIXEL_SIZE),
    .pointsOffset = {
        [0] = 0,
        [1] = PLAYER_WIDTH / 2,
        [2] = PLAYER_WIDTH - PIXEL_SIZE
    }
};

static const struct HitboxData sHitboxBottom = {
    .axisOffset = PIXEL_SIZE,
    .pointsOffset = {
        [0] = 0,
        [1] = PLAYER_WIDTH / 2,
        [2] = PLAYER_WIDTH - PIXEL_SIZE
    }
};

static void HandleLeftCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.x + sHitboxLeft.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[0]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[1]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[2]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[3]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[4]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[5]) == COLLISION_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.right;
    }
}

static void HandleRightCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.x + sHitboxRight.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[0]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[1]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[2]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[3]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[4]) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[5]) == COLLISION_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.left - PLAYER_WIDTH;
    }
}

static void HandleBottomCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.y + sHitboxBottom.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[0], mainAxis) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[1], mainAxis) == COLLISION_SOLID || 
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[2], mainAxis) == COLLISION_SOLID)
    {
        gPlayerMovement.yVelocity = 0;
        gPlayerMovement.grounded = TRUE;
        gPlayerData.y = gCollisionInfo.top;
    }
    else
    {
        gPlayerMovement.grounded = FALSE;
    }
}

static void HandleTopCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.y + sHitboxTop.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[0], mainAxis) == COLLISION_SOLID ||
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[1], mainAxis) == COLLISION_SOLID ||
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[2], mainAxis) == COLLISION_SOLID)
    {
        gPlayerMovement.yVelocity = 0;
        gPlayerData.y = gCollisionInfo.bottom + PLAYER_HEIGHT;
    }
}

void PlayerHandleTerrainCollision(void)
{
    if (gPlayerMovement.xVelocity < 0)
        HandleLeftCollision();
    else if (gPlayerMovement.xVelocity != 0)
        HandleRightCollision();

    if (gPlayerMovement.yVelocity < 0)
        HandleTopCollision();
    else if (gPlayerMovement.yVelocity != 0)
        HandleBottomCollision();
}
