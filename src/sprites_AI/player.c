#include "sprites_AI/player.h"

#include "gb/oam.h"

#include "player.h"
#include "input.h"
#include "bg.h"
#include "bg_clip.h"
#include "io.h"
#include "bg_clip.h"
#include "sprite.h"
#include "macros.h"

static const u8 sPlayerOam_Frame0[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-8),  OAM_POS(0), 0x19, 0,
    OAM_POS(-8),  OAM_POS(8), 0x19, 0,
    OAM_POS(-16), OAM_POS(0), 0x19, 0,
    OAM_POS(-16), OAM_POS(8), 0x19, 0,
    OAM_POS(-24), OAM_POS(0), 0x19, 0,
    OAM_POS(-24), OAM_POS(8), 0x19, 0,
};

static const struct AnimData sPlayerAnim[] = {
    [0] = {
        .oamPointer = sPlayerOam_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

struct HitboxData {
    s8 axisOffset;
    s8 pointsOffset[3];
};

#define PLAYER_HEIGHT (BLOCK_SIZE * 3)
#define PLAYER_WIDTH  (BLOCK_SIZE * 2)

static const struct HitboxData sHitboxLeft = {
    .axisOffset = -QUARTER_BLOCK_SIZE,
    .pointsOffset = {
        [0] = -PLAYER_HEIGHT / 4,
        [1] = -PLAYER_HEIGHT / 2,
        [2] = -(PLAYER_HEIGHT - PIXEL_SIZE)
    }
};

static const struct HitboxData sHitboxRight = {
    .axisOffset = PLAYER_WIDTH,
    .pointsOffset = {
        [0] = -PLAYER_HEIGHT / 4,
        [1] = -PLAYER_HEIGHT / 2,
        [2] = -(PLAYER_HEIGHT - PIXEL_SIZE)
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

static void HandleHorizontalMovement(void)
{
    if (gButtonInput & KEY_LEFT)
    {
        gPlayerMovement.xVelocity -= gPlayerPhysics.xAcceleration;
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
        if (gPlayerMovement.xVelocity > gPlayerPhysics.xVelocityCap)
            gPlayerMovement.xVelocity = gPlayerPhysics.xVelocityCap;
    }
    else
    {
        if (gPlayerMovement.xVelocity > 0)
            gPlayerMovement.xVelocity = 0;
    }
}

static void HandleVerticalMovement(void)
{
    if (gPlayerMovement.grounded && gChangedInput & KEY_B)
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

static void HandleLeftCollision(void)
{
    u16 mainAxis;

    mainAxis = gCurrentSprite.x + sHitboxLeft.axisOffset;

    if (GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxLeft.pointsOffset[0]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxLeft.pointsOffset[1]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxLeft.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gCurrentSprite.x = gCollisionInfo.right;
    }
}

static void HandleRightCollision(void)
{
    u16 mainAxis;

    mainAxis = gCurrentSprite.x + sHitboxRight.axisOffset;

    if (GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxRight.pointsOffset[0]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxRight.pointsOffset[1]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gCurrentSprite.y + sHitboxRight.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gCurrentSprite.x = gCollisionInfo.left - PLAYER_WIDTH;
    }
}

void HandleBottomCollision(void)
{
    u16 mainAxis;

    mainAxis = gCurrentSprite.y + sHitboxBottom.axisOffset;

    if (GetClipdataValue(gCurrentSprite.x + sHitboxBottom.pointsOffset[0], mainAxis) == CLIPDATA_SOLID || 
        GetClipdataValue(gCurrentSprite.x + sHitboxBottom.pointsOffset[1], mainAxis) == CLIPDATA_SOLID || 
        GetClipdataValue(gCurrentSprite.x + sHitboxBottom.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
    {
        gPlayerMovement.yVelocity = 0;
        gPlayerMovement.grounded = TRUE;
        gCurrentSprite.y = gCollisionInfo.top;
    }
    else
    {
        gPlayerMovement.grounded = FALSE;
    }
}

static void HandleTopCollision(void)
{
    u16 mainAxis;

    mainAxis = gCurrentSprite.y + sHitboxTop.axisOffset;

    if (GetClipdataValue(gCurrentSprite.x + sHitboxTop.pointsOffset[0], mainAxis) == CLIPDATA_SOLID ||
        GetClipdataValue(gCurrentSprite.x + sHitboxTop.pointsOffset[1], mainAxis) == CLIPDATA_SOLID ||
        GetClipdataValue(gCurrentSprite.x + sHitboxTop.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
    {
        gPlayerMovement.yVelocity = 0;
        gCurrentSprite.y = gCollisionInfo.bottom + PLAYER_HEIGHT;
    }
}

static void HandleTerrainCollision(void)
{
    if (gPlayerMovement.xVelocity < 0)
        HandleLeftCollision();
    else if (gPlayerMovement.xVelocity > 0)
        HandleRightCollision();

    if (gPlayerMovement.yVelocity < 0)
        HandleTopCollision();
    else if (gPlayerMovement.yVelocity > 0)
        HandleBottomCollision();
}

static void ApplyMovement(void)
{
    gCurrentSprite.x += gPlayerMovement.xVelocity;
    gCurrentSprite.y += gPlayerMovement.yVelocity;
}

static void HandleScrolling(void)
{
    u16 x;
    
    x = SUB_PIXEL_TO_PIXEL(gCurrentSprite.x) - gBackgroundInfo.x;

    if (x > SCREEN_SIZE_X / 2 || x < SCREEN_SIZE_X / 2)
        gBackgroundInfo.x += SUB_PIXEL_TO_PIXEL(gPlayerMovement.xVelocity);
}

void Player(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sPlayerAnim;
        gCurrentSprite.pose = 1;
    }

    HandleHorizontalMovement();
    HandleVerticalMovement();
    HandleTerrainCollision();
    ApplyMovement();
    HandleScrolling();
}
