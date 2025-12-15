#include "player/player.h"

#include "gb/oam.h"
#include "gb/io.h"

#include "input.h"
#include "callbacks.h"
#include "bg.h"
#include "bg_clip.h"
#include "io.h"
#include "sprite.h"
#include "fading.h"
#include "game_state.h"
#include "macros.h"
#include "player/player_movement.h"
#include "player/player_collisions.h"

struct PlayerData gPlayerData;

extern const struct AnimData sPlayerAnim_Idle[];
extern const u8 sPlayerGraphics[];

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
    gPlayerData.animTimer += DELTA_TIME;

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

    if (gChangedInput & KEY_UP)
    {
        left = GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y - PLAYER_HEIGHT - PIXEL_SIZE);
        right = GET_CLIPDATA_BEHAVIOR(gPlayerData.x, gPlayerData.y - PLAYER_HEIGHT - PIXEL_SIZE);
    
        if (left != CLIP_BEHAVIOR_LADDER && right != CLIP_BEHAVIOR_LADDER)
            return;
    }
    else if (gChangedInput & KEY_DOWN)
    {
        left = GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y + BLOCK_SIZE);
        right = GET_CLIPDATA_BEHAVIOR(gPlayerData.x, gPlayerData.y + BLOCK_SIZE);

        if (left != CLIP_BEHAVIOR_LADDER && right != CLIP_BEHAVIOR_LADDER)
            return;

        gPlayerData.y &= BLOCK_POSITION_FLAG;
        gPlayerData.y += PLAYER_HEIGHT / 2;
    }
    else
    {
        return;
    }

    PlayerSetPose(PLAYER_POSE_ON_LADDER);

    gPlayerData.x &= BLOCK_POSITION_FLAG;

    if (left != CLIP_BEHAVIOR_LADDER)
        gPlayerData.x -= BLOCK_SIZE;
    else if (right != CLIP_BEHAVIOR_LADDER)
        gPlayerData.x += BLOCK_SIZE;
}

void PlayerInit(void)
{
    PlayerInitPhysics();
    PlayerLoadGraphics();

    gPlayerData.x = BLOCK_SIZE * 9;
    gPlayerData.y = BLOCK_SIZE * 13;

    PlayerSetPose(PLAYER_POSE_IDLE);
    gPlayerData.animPointer = sPlayerAnim_Idle;
}

void PlayerSetPose(u8 pose)
{
    gPlayerData.pose = pose;
    gPlayerData.currentAnimFrame = 0;
    gPlayerData.animTimer = 0;
}

void PlayerUpdate(void)
{
    switch (gPlayerData.pose)
    {
        case PLAYER_POSE_ON_LADDER:
            PlayerLadderMovement();
            break;

        default:
            PlayerHorizontalMovement();
            PlayerVerticalMovement();
            PlayerHandleTerrainCollision();
            PlayerApplyMovement();

            CheckForLadder();
    }

    if (gPlayerMovement.direction & KEY_LEFT)
        gPlayerData.properties &= ~SPRITE_PROPERTY_X_FLIP;
    else
        gPlayerData.properties |= SPRITE_PROPERTY_X_FLIP;

    UpdateAnimation();
}

void PlayerDraw(void)
{
    u8* oam;
    const u8* oamData;
    u8 partCount;
    u8 i;
    u8 x;
    u8 y;
    u8 properties;

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
    properties = (gPlayerData.properties & SPRITE_PROPERTY_GFX) << 4;

    for (i = 0; i < partCount; i++)
    {
        // Copy all of the raw oam and add the attributes

        // Y position
        *oam++ = (*oamData++) + y;
        
        // X position
        if (gPlayerData.properties & SPRITE_PROPERTY_X_FLIP)
        {
            // If the sprite is X flipped, we need to apply the oam X offset in the opposite direction
            // as well as account for a full tile, hence the - 8
            *oam = x - (*oamData - 8);
        }
        else
        {
            *oam = (*oamData) + x;
        }

        oam++;
        oamData++;

        // Tile index
        *oam++ = (*oamData++);
        // Attribute flags
        *oam++ = (*oamData++) ^ properties;
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

static const u8 sPlayerAnim_Idle_Frame0[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-24), OAM_POS(8), 4, 0,
    OAM_POS(-24), OAM_POS(0), 1, 0,
    OAM_POS(-16), OAM_POS(8), 5, 0,
    OAM_POS(-16), OAM_POS(0), 2, 0,
    OAM_POS(-8), OAM_POS(8), 6, 0,
    OAM_POS(-8), OAM_POS(0), 3, 0,
};

static const u8 sPlayerAnim_Idle_Frame1[OAM_DATA_SIZE(6)] = {
    6,
    OAM_POS(-22), OAM_POS(8), 4, 0,
    OAM_POS(-22), OAM_POS(0), 1, 0,
    OAM_POS(-14), OAM_POS(8), 5, 0,
    OAM_POS(-14), OAM_POS(0), 2, 0,
    OAM_POS(-8), OAM_POS(8), 8, 0,
    OAM_POS(-8), OAM_POS(0), 7, 0,
};

const struct AnimData sPlayerAnim_Idle[] = {
    [0] = {
        .oamPointer = sPlayerAnim_Idle_Frame0,
        .duration = 50,
    },
    [1] = {
        .oamPointer = sPlayerAnim_Idle_Frame1,
        .duration = 50,
    },
    [2] = SPRITE_ANIM_TERMINATOR
};
