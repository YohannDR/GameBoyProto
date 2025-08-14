#include "player.h"

#include "gb/oam.h"
#include "gb/io.h"

#include "input.h"
#include "callbacks.h"
#include "bg.h"
#include "bg_clip.h"
#include "io.h"
#include "sprite.h"
#include "fading.h"
#include "macros.h"

struct PlayerData gPlayerData;
struct PlayerPhysics gPlayerPhysics;
struct PlayerMovement gPlayerMovement;

extern const struct AnimData sPlayerAnim[];
extern const u8 sPlayerGraphics[];

#define LADDER_SPEED (PIXEL_SIZE)

struct HitboxData {
    s8 axisOffset;
    s8 pointsOffset[3];
};

enum PlayerPose {
    POSE_IDLE,
    POSE_RUNNING,
    POSE_JUMPING,
    POSE_ON_LADDER,
};

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

static void PlayerInitPhysics(void)
{
    gPlayerPhysics.xAcceleration = 1;
    gPlayerPhysics.xVelocityCap = 8;
    gPlayerPhysics.yVelocityCap = 8;
    gPlayerPhysics.gravityUpwards = 1;
    gPlayerPhysics.gravityDownwards = 2;
    gPlayerPhysics.jumpingVelocity = -16;
}

static void PlayerSetPose(u8 pose)
{
    gPlayerData.pose = pose;
}

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

    mainAxis = gPlayerData.x + sHitboxLeft.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[0]) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[1]) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.right;
    }
}

static void HandleRightCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.x + sHitboxRight.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[0]) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[1]) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.left - PLAYER_WIDTH;
    }
}

static void HandleBottomCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.y + sHitboxBottom.axisOffset;

    if (GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[0], mainAxis) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[1], mainAxis) == CLIPDATA_SOLID || 
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxBottom.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
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

    if (GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[0], mainAxis) == CLIPDATA_SOLID ||
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[1], mainAxis) == CLIPDATA_SOLID ||
        GET_CLIPDATA_SOLIDITY(gPlayerData.x + sHitboxTop.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
    {
        gPlayerMovement.yVelocity = 0;
        gPlayerData.y = gCollisionInfo.bottom + PLAYER_HEIGHT;
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
    gPlayerData.x += gPlayerMovement.xVelocity;
    gPlayerData.y += gPlayerMovement.yVelocity;
}

static void PlayerLoadGraphics(void)
{
    const u8* src;
    u8* dst;
    u8 tileCount;
    u8 i;

    src = sPlayerGraphics;
    tileCount = *src++;

    dst = (u8*)VRAM_BASE;

    for (i = 0; i < tileCount; i++)
    {
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
    }
}

static void UpdateAnimation(void)
{
    const struct AnimData* anim;

#ifdef HACKY_OPTIMIZATIONS
    anim = HACKY_ARRAY_INDEXING(gPlayerData.animPointer, gPlayerData.currentAnimFrame, struct AnimData);
#else
    anim = &gPlayerData.animPointer[gPlayerData.currentAnimFrame];
#endif
    gPlayerData.animTimer++;

    if (gPlayerData.animTimer >= anim->duration)
    {
        gPlayerData.animTimer = 0;
        gPlayerData.currentAnimFrame++;
        anim++;

        if (anim->duration == 0)
            gPlayerData.currentAnimFrame = 0;
    }
}

static void CheckForLadder(void)
{
    u8 left;
    u8 right;

    if (!(gChangedInput & KEY_UP))
        return;

    left = GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y - PLAYER_HEIGHT - PIXEL_SIZE);
    right = GET_CLIPDATA_BEHAVIOR(gPlayerData.x, gPlayerData.y - PLAYER_HEIGHT - PIXEL_SIZE);

    if (left == CLIP_BEHAVIOR_LADDER || right == CLIP_BEHAVIOR_LADDER)
    {
        PlayerSetPose(POSE_ON_LADDER);

        gPlayerData.x &= BLOCK_POSITION_FLAG;

        if (left != CLIP_BEHAVIOR_LADDER)
            gPlayerData.x -= BLOCK_SIZE;
        else if (right != CLIP_BEHAVIOR_LADDER)
            gPlayerData.x += BLOCK_SIZE;
    }
}

static void PlayerOnLadder(void)
{
    if (gButtonInput & KEY_DOWN)
    {
        gPlayerData.y += LADDER_SPEED;

        if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y) != CLIP_BEHAVIOR_LADDER)
            PlayerSetPose(POSE_IDLE);
    }
    else if (gButtonInput & KEY_UP)
    {
        gPlayerData.y -= LADDER_SPEED;

        if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y - PIXEL_SIZE) != CLIP_BEHAVIOR_LADDER)
            PlayerSetPose(POSE_IDLE);
    }
}

void PlayerInit(void)
{
    PlayerInitPhysics();
    PlayerLoadGraphics();

    gPlayerData.x = BLOCK_SIZE * 3;
    gPlayerData.y = BLOCK_SIZE * 9;
    gPlayerData.animPointer = sPlayerAnim;
}

void PlayerUpdate(void)
{
    switch (gPlayerData.pose)
    {
        case POSE_ON_LADDER:
            PlayerOnLadder();
            break;

        default:
            HandleHorizontalMovement();
            HandleVerticalMovement();
            HandleTerrainCollision();
            ApplyMovement();
            CheckForLadder();
    }

    UpdateAnimation();

    if (gChangedInput & KEY_A)
    {
        if (gPlayerData.work1)
        {
            FadingStart(FADING_TARGET_BACKGROUND, gBackgroundPalette, 3);
            FadingStart(FADING_TARGET_OBJ0, gObj0Palette, 3);
        }
        else
        {
            FadingStart(FADING_TARGET_BACKGROUND, PALETTE_WHITE, 3);
            FadingStart(FADING_TARGET_OBJ0, PALETTE_BLACK, 3);
        }

        gPlayerData.work1 ^= 1;
    }
}

void PlayerDraw(void)
{
    u8* oam;
    const u8* oamData;
    u8 partCount;
    u8 i;
    u8 x;
    u8 y;

    // Get the target slot in the oam buffer
    oam = OAM_BUFFER_SLOT(gNextOamSlot);

    // Get the sprite's current oam data
#ifdef HACKY_OPTIMIZATIONS
    oamData = HACKY_ARRAY_INDEXING(gPlayerData.animPointer, gPlayerData.currentAnimFrame, struct AnimData)->oamPointer;
#else
    oamData = gPlayerData.animPointer[gPlayerData.currentAnimFrame].oamPointer;
#endif

    // Get the part count, it's always the first element
    partCount = *oamData++;

    // Get the sprite's attributes
    x = SUB_PIXEL_TO_PIXEL(gPlayerData.x) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    y = SUB_PIXEL_TO_PIXEL(gPlayerData.y) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);

    for (i = 0; i < partCount; i++)
    {
        // Copy all of the raw oam and add the attributes

        // Y position
        *oam++ = (*oamData++) + y;
        // X position
        *oam++ = (*oamData++) + x;
        // Tile index
        *oam++ = (*oamData++);
        // Attribute flags
        *oam++ = (*oamData++);
    }

    // Update the next free oam slot
    gNextOamSlot += partCount;
}

const u8 sPlayerGraphics[] = {
    9,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x1F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xF8, 0xFF, 0xF1, 0xFE, 0xEF, 0xF0, 0xFF, 0xE0,
    0x5B, 0x64, 0x79, 0x66, 0x7F, 0x00, 0x3F, 0x00, 0x1F, 0x00, 0x0C, 0x37, 0x0C, 0x77, 0x6D, 0x37,
    0x0C, 0x77, 0x0C, 0x77, 0x0C, 0x77, 0x4C, 0x37, 0x1F, 0x27, 0x11, 0x29, 0x00, 0x79, 0x00, 0x79,
    0xC8, 0xC0, 0xE8, 0xE0, 0xF8, 0xF0, 0xF8, 0xF0, 0xF0, 0x78, 0x38, 0xC8, 0x38, 0xC8, 0xB8, 0x48,
    0xF8, 0x38, 0xF0, 0x30, 0xE0, 0x00, 0xC0, 0x00, 0x80, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x80, 0x60,
    0x00, 0xE0, 0x80, 0xE0, 0x80, 0xE0, 0xE0, 0x80, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0,
    0x0C, 0x77, 0x0C, 0x77, 0x0C, 0x77, 0x0C, 0x77, 0x0C, 0x77, 0x5F, 0x27, 0x11, 0x29, 0x00, 0x79,
    0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE0, 0x80, 0xE0, 0x80, 0xE0, 0xE0, 0x80, 0x00, 0xC0, 0x00, 0xC0,
};

static const u8 sPlayerAnim_Frame0[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-24), OAM_POS(8), 4, 0,
    OAM_POS(-24), OAM_POS(0), 1, 0,
    OAM_POS(-16), OAM_POS(8), 5, 0,
    OAM_POS(-16), OAM_POS(0), 2, 0,
    OAM_POS(-8), OAM_POS(8), 6, 0,
    OAM_POS(-8), OAM_POS(0), 3, 0,
};

static const u8 sPlayerAnim_Frame1[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-22), OAM_POS(8), 4, 0,
    OAM_POS(-22), OAM_POS(0), 1, 0,
    OAM_POS(-14), OAM_POS(8), 5, 0,
    OAM_POS(-14), OAM_POS(0), 2, 0,
    OAM_POS(-8), OAM_POS(8), 8, 0,
    OAM_POS(-8), OAM_POS(0), 7, 0,
};

const struct AnimData sPlayerAnim[] = {
    [0] = {
        .oamPointer = sPlayerAnim_Frame0,
        .duration = 50,
    },
    [1] = {
        .oamPointer = sPlayerAnim_Frame1,
        .duration = 50,
    },
    [2] = SPRITE_ANIM_TERMINATOR
};
