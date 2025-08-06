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

static void PlayerInitPhysics(void)
{
    gPlayerPhysics.xAcceleration = ONE_SUB_PIXEL;
    gPlayerPhysics.xVelocityCap = QUARTER_BLOCK_SIZE;
    gPlayerPhysics.yVelocityCap = QUARTER_BLOCK_SIZE;

    gPlayerPhysics.gravityUpwards = ONE_SUB_PIXEL;
    gPlayerPhysics.gravityDownwards = PIXEL_SIZE / 2;

    gPlayerPhysics.jumpingVelocity = -HALF_BLOCK_SIZE;
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

    if (GetClipdataValue(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[0]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[1]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gPlayerData.y + sHitboxLeft.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.right;
    }
}

static void HandleRightCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.x + sHitboxRight.axisOffset;

    if (GetClipdataValue(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[0]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[1]) == CLIPDATA_SOLID || 
        GetClipdataValue(mainAxis, gPlayerData.y + sHitboxRight.pointsOffset[2]) == CLIPDATA_SOLID)
    {
        gPlayerMovement.xVelocity = 0;
        gPlayerData.x = gCollisionInfo.left - PLAYER_WIDTH;
    }
}

static void HandleBottomCollision(void)
{
    u16 mainAxis;

    mainAxis = gPlayerData.y + sHitboxBottom.axisOffset;

    if (GetClipdataValue(gPlayerData.x + sHitboxBottom.pointsOffset[0], mainAxis) == CLIPDATA_SOLID || 
        GetClipdataValue(gPlayerData.x + sHitboxBottom.pointsOffset[1], mainAxis) == CLIPDATA_SOLID || 
        GetClipdataValue(gPlayerData.x + sHitboxBottom.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
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

    if (GetClipdataValue(gPlayerData.x + sHitboxTop.pointsOffset[0], mainAxis) == CLIPDATA_SOLID ||
        GetClipdataValue(gPlayerData.x + sHitboxTop.pointsOffset[1], mainAxis) == CLIPDATA_SOLID ||
        GetClipdataValue(gPlayerData.x + sHitboxTop.pointsOffset[2], mainAxis) == CLIPDATA_SOLID)
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

void PlayerInit(void)
{
    PlayerInitPhysics();
    PlayerLoadGraphics();

    gPlayerData.x = BLOCK_SIZE * 8;
    gPlayerData.y = BLOCK_SIZE * 17;
}

void PlayerUpdate(void)
{
    if (gPlayerData.pose == 0)
    {
        gPlayerData.animPointer = sPlayerAnim;
        gPlayerData.pose = 1;
    }

    HandleHorizontalMovement();
    HandleVerticalMovement();
    HandleTerrainCollision();
    ApplyMovement();

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
    1,

    0x3C, 0x00, 0x42, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0x42, 0x00, 0x3C, 0x00,
};

static const u8 sPlayerAnim_Frame0[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-8), OAM_POS(0), 0, 0,
    OAM_POS(-8), OAM_POS(8), 0, 0,
    OAM_POS(-16), OAM_POS(0), 0, 0,
    OAM_POS(-16), OAM_POS(8), 0, 0,
    OAM_POS(-24), OAM_POS(0), 0, 0,
    OAM_POS(-24), OAM_POS(8), 0, 0,
};

static const u8 sPlayerAnim_Frame1[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-8), OAM_POS(0), 0, 0,
    OAM_POS(-8), OAM_POS(8), 0, 0,
    OAM_POS(-16), OAM_POS(0), 0, 0,
    OAM_POS(-16), OAM_POS(8), 0, 0,
    OAM_POS(-24), OAM_POS(0), 0, 0,
    OAM_POS(-24), OAM_POS(8), 0, 0,
};

const struct AnimData sPlayerAnim[] = {
    [0] = {
        .oamPointer = sPlayerAnim_Frame0,
        .duration = 100,
    },
    [1] = {
        .oamPointer = sPlayerAnim_Frame1,
        .duration = 100,
    },
    [2] = SPRITE_ANIM_TERMINATOR
};
