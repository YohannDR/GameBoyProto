#include "fire.h"

#include "gb/oam.h"
#include "gb/io.h"

#include "bg.h"
#include "bg_clip.h"
#include "game_state.h"
#include "macros.h"
#include "math.h"
#include "sprite.h"

/**
 * @brief Whether the fire cluster exists
 * 
 */
#define FIRE_STATUS_EXISTS      (1u << 0)

/**
 * @brief Whether the fire cluster is expanding up
 * 
 */
#define FIRE_STATUS_UP          (1u << 1)

/**
 * @brief Whether the fire cluster is expanding down
 * 
 */
#define FIRE_STATUS_DOWN        (1u << 2)

/**
 * @brief Whether the fire cluster is expanding left
 * 
 */
#define FIRE_STATUS_LEFT        (1u << 3)

/**
 * @brief Whether the fire cluster is expanding right
 * 
 */
#define FIRE_STATUS_RIGHT       (1u << 4)

/**
 * @brief Whether the fire cluster is locked, aka can't expand
 * 
 */
#define FIRE_STATUS_LOCKED      (1u << 5)

/**
 * @brief Whether the fire should also propagate backwards, e.g. it's going up, but it can go down too
 * 
 */
#define FIRE_STATUS_BACKWARD    (1u << 6)

/**
 * @brief Dictates in which direction the fire is currently propagating in case it can both forward and backwards (thus FIRE_STATUS_BACKWARD has to be set)
 * 
 */
#define FIRE_STATUS_DIRECTION   (1u << 7)

#define FIRE_DIRECTION_MASK (FIRE_STATUS_UP | FIRE_STATUS_DOWN | FIRE_STATUS_LEFT | FIRE_STATUS_RIGHT)

#define FIRE_SPREADING_DURATION (CONVERT_SECONDS(4.f))
#define FIRE_MAX_LENGTH (10)

#define FIRE_GFX_SLOT (112)

#define FIRE_TILE_EXISTS (1u << 7)

struct FireCluster {
    u8 status;
    u16 x;
    u16 y;
    u8 spreadTimer;
    u8 length;
};

struct FireTile {
    u8 parentCluster;
    u16 x;
    u16 y;
};

static struct FireCluster gFireClusters[4];
static struct FireCluster gCurrentFire;
static u8 gCurrentClusterId;
static u8 gMaxAmountOfExistingFire;

static u8 gAnimationTimer;
static u8 gCurrentAnimationFrame;

static struct FireTile gFireTiles[50];
static u8 gMaxAmountOfExistingFireTiles;

extern const u8 sFireGraphics[];
extern const struct AnimData sFireAnim_Idle[];

static void FindFireDirection(void)
{
    u16 x;
    u16 y;

    x = gCurrentFire.x;
    y = gCurrentFire.y;

    // We need to check all 4 cardinal directions

    // Check up
    if (GET_CLIPDATA_BEHAVIOR(x, y - BLOCK_SIZE) == CLIP_BEHAVIOR_INFLAMMABLE)
        gCurrentFire.status |= FIRE_STATUS_UP;

    // Check down
    if (GET_CLIPDATA_BEHAVIOR(x, y + BLOCK_SIZE) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        // If the fire was already determined to be going up, we mark it as going backwards too, otherwise it's just going down
        if (gCurrentFire.status & FIRE_STATUS_UP)
            gCurrentFire.status |= FIRE_STATUS_BACKWARD;
        else
            gCurrentFire.status |= FIRE_STATUS_DOWN;

        return;
    }

    // If we found a direction we can abort
    if (gCurrentFire.status & FIRE_DIRECTION_MASK)
        return;

    // Same logic is applied to find a potential horizontal direction

    // Check left
    if (GET_CLIPDATA_BEHAVIOR(x - BLOCK_SIZE, y) == CLIP_BEHAVIOR_INFLAMMABLE)
        gCurrentFire.status |= FIRE_STATUS_LEFT;

    // Check right
    if (GET_CLIPDATA_BEHAVIOR(x + BLOCK_SIZE, y) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        // If the fire was already determined to be going left, we mark it as going backwards too, otherwise it's just going right
        if (gCurrentFire.status & FIRE_STATUS_LEFT)
            gCurrentFire.status |= FIRE_STATUS_BACKWARD;
        else
            gCurrentFire.status |= FIRE_STATUS_RIGHT;

        return;
    }

    // If we found a direction we can abort
    if (gCurrentFire.status & FIRE_DIRECTION_MASK)
        return;

    // No direction was found, so the fire can't expand
    gCurrentFire.status |= FIRE_STATUS_LOCKED;
}

static void FireUpdateAnimation(void)
{
    const struct AnimData* anim;

#ifdef HACKY_OPTIMIZATIONS
    anim = HACKY_ARRAY_INDEXING(sFireAnim_Idle, gCurrentAnimationFrame, struct AnimData);
#else
    anim = &animPointer[gCurrentAnimationFrame];
#endif
    gAnimationTimer++;

    if (gAnimationTimer >= anim->duration)
    {
        gAnimationTimer = 0;
        gCurrentAnimationFrame++;
        anim++;

        if (anim->duration == 0)
            gCurrentAnimationFrame = 0;
    }
}

static void BurnTile(u16 x, u16 y)
{
    u8 i;

    for (i = 0; i < ARRAY_SIZE(gFireTiles); i++)
    {
        if (gFireTiles[i].parentCluster & FIRE_TILE_EXISTS)
            continue;

        gFireTiles[i].parentCluster = gCurrentClusterId | FIRE_TILE_EXISTS;
        gFireTiles[i].x = x;
        gFireTiles[i].y = y;

        if (i >= gMaxAmountOfExistingFireTiles)
            gMaxAmountOfExistingFireTiles = i + 1;

        return;
    }
}

static void MergeClusters(u8 otherCluster)
{
    struct FireCluster* cluster;

    cluster = &gFireClusters[otherCluster];

    // Kill the other cluster
    cluster->status = 0;
    // Append the other cluster length
    gCurrentFire.length += cluster->length + 1;
}

static void PropagateFire(void)
{
    u16 x;
    u16 y;
    u16 offset;
    u8 otherCluster;

    gCurrentFire.spreadTimer += DELTA_TIME;

    if (gCurrentFire.spreadTimer != FIRE_SPREADING_DURATION)
        return;

    gCurrentFire.spreadTimer = 0;

    if (gCurrentFire.status & FIRE_STATUS_BACKWARD)
    {
        if (gCurrentFire.status & FIRE_STATUS_DIRECTION)
        {
            // Going backwards, we check "behind" the starting position
            x = gCurrentFire.x;
            y = gCurrentFire.y;

            // Check only one block behind
            if (gCurrentFire.status & FIRE_STATUS_UP)
                y += BLOCK_SIZE;
            else if (gCurrentFire.status & FIRE_STATUS_DOWN)
                y -= BLOCK_SIZE;
            else if (gCurrentFire.status & FIRE_STATUS_LEFT)
                x += BLOCK_SIZE;
            else if (gCurrentFire.status & FIRE_STATUS_RIGHT)
                x -= BLOCK_SIZE;

            if (GET_CLIPDATA_BEHAVIOR(x, y) == CLIP_BEHAVIOR_INFLAMMABLE)
            {
                // Move the starting point backwards to the newly located block
                if (gCurrentFire.status & (FIRE_STATUS_UP | FIRE_STATUS_DOWN))
                    gCurrentFire.y = y;
                else if (gCurrentFire.status & (FIRE_STATUS_LEFT | FIRE_STATUS_RIGHT))
                    gCurrentFire.x = x;

                // And increase the length
                gCurrentFire.length++;
                if (gCurrentFire.length == FIRE_MAX_LENGTH)
                    gCurrentFire.status |= FIRE_STATUS_LOCKED;

                BurnTile(gCurrentFire.x, gCurrentFire.y);

                // Toggle and return
                gCurrentFire.status ^= FIRE_STATUS_DIRECTION;
                return;
            }

            // If we didn't find a inflammable block, we can remove the backward flag, and fall-through the normal "forward" check
            gCurrentFire.status &= ~FIRE_STATUS_DIRECTION;
        }

        // Swap the direction
        gCurrentFire.status ^= FIRE_STATUS_DIRECTION;
    }

    x = gCurrentFire.x;
    y = gCurrentFire.y;
    // Check the next block in the chain
    offset = (gCurrentFire.length + 1) * BLOCK_SIZE;

    if (gCurrentFire.status & FIRE_STATUS_UP)
        y -= offset;
    else if (gCurrentFire.status & FIRE_STATUS_DOWN)
        y += offset;
    else if (gCurrentFire.status & FIRE_STATUS_LEFT)
        x -= offset;
    else if (gCurrentFire.status & FIRE_STATUS_RIGHT)
        x += offset;

    // Check if we're colliding with another fire cluster
    otherCluster = IsTileBurned(x, y);

    if (otherCluster)
    {
        // Get the cluster id
        otherCluster--;
        MergeClusters(otherCluster);
        return;
    }

    if (GET_CLIPDATA_BEHAVIOR(x, y) != CLIP_BEHAVIOR_INFLAMMABLE)
    {
        // There's no more inflammable blocks, so we have reached the end of the propagation
        gCurrentFire.status |= FIRE_STATUS_LOCKED;
        return;
    }
    
    BurnTile(x, y);

    gCurrentFire.length++;
    if (gCurrentFire.length == FIRE_MAX_LENGTH)
        gCurrentFire.status |= FIRE_STATUS_LOCKED;

}

static void FireDraw(void)
{
    const u8* oamData;
    u8* dst;
    u8 i;
    u8 y;
    u8 x;

    // TODO Add check to not draw if off screen

    dst = OAM_BUFFER_SLOT(gNextOamSlot);

    // Get the sprite's current oam data
#ifdef HACKY_OPTIMIZATIONS
    oamData = HACKY_ARRAY_INDEXING(sFireAnim_Idle, gCurrentAnimationFrame, struct AnimData)->oamPointer;
#else
    oamData = sFireAnim_Idle[gCurrentAnimationFrame].oamPointer;
#endif

    // We ignore the part count, as fire is hard coded to only draw a single tile
    oamData++;
    
    // Get the fire start attributes
    x = SUB_PIXEL_TO_PIXEL(gCurrentFire.x) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    y = SUB_PIXEL_TO_PIXEL(gCurrentFire.y) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);

    for (i = 0; i < gCurrentFire.length + 1; i++)
    {
        *dst++ = (*oamData++) + y;
        *dst++ = (*oamData++) + x;
        *dst++ = (*oamData++) + FIRE_GFX_SLOT;
        *dst++ = (*oamData++);

        oamData -= 4;

        if (gCurrentFire.status & FIRE_STATUS_UP)
            y -= SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        else if (gCurrentFire.status & FIRE_STATUS_DOWN)
            y += SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        else if (gCurrentFire.status & FIRE_STATUS_LEFT)
            x -= SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        else if (gCurrentFire.status & FIRE_STATUS_RIGHT)
            x += SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
    }

    gNextOamSlot += gCurrentFire.length + 1;
}

void LoadFireGraphics(void)
{
    u8* dst;
    const u8* src;
    u8 tileCount;
    u8 i;

    dst = (u8*)(VRAM_BASE + FIRE_GFX_SLOT * 16);

    src = sFireGraphics;
    tileCount = *src++;

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

void StartFire(u16 x, u16 y)
{
    u8 i;

    if (IsTileBurned(x, y))
        return;

    for (i = 0; i < ARRAY_SIZE(gFireClusters); i++)
    {
        if (gFireClusters[i].status & FIRE_STATUS_EXISTS)
            continue;

        gFireClusters[i].status = FIRE_STATUS_EXISTS;
        // Normalize fire position in the middle of the block
        gFireClusters[i].x = (x & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
        gFireClusters[i].y = (y & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
        gFireClusters[i].spreadTimer = 0;
        gFireClusters[i].length = 0;

        gCurrentClusterId = i;

        BurnTile(gFireClusters[i].x, gFireClusters[i].y);

        if (i >= gMaxAmountOfExistingFire)
            gMaxAmountOfExistingFire = i + 1;

        return;
    }
}

void SpawnCluster(u16 x, u16 y, u8 direction, u8 length)
{
    u8 i;

    for (i = 0; i < ARRAY_SIZE(gFireClusters); i++)
    {
        if (gFireClusters[i].status & FIRE_STATUS_EXISTS)
            continue;

        // Hard spawned clusters are assumed to not spread
        gFireClusters[i].status = FIRE_STATUS_EXISTS | FIRE_STATUS_LOCKED | (direction << 1);
        // Normalize fire position in the middle of the block
        gFireClusters[i].x = (x & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
        gFireClusters[i].y = (y & BLOCK_POSITION_FLAG) - HALF_BLOCK_SIZE;
        gFireClusters[i].spreadTimer = 0;
        gFireClusters[i].length = length;
        gCurrentClusterId = i;

        if (i >= gMaxAmountOfExistingFire)
            gMaxAmountOfExistingFire = i + 1;

        x = gFireClusters[i].x;
        y = gFireClusters[i].y;

        // i can safely be re-used here, we're exiting the loop afterwards
        for (i = 0; i < length + 1; i++)
        {
            BurnTile(x, y);

            if (gFireClusters[i].status & FIRE_STATUS_UP)
                y -= BLOCK_SIZE;
            else if (gFireClusters[i].status & FIRE_STATUS_DOWN)
                y += BLOCK_SIZE;
            else if (gFireClusters[i].status & FIRE_STATUS_LEFT)
                x -= BLOCK_SIZE;
            else if (gFireClusters[i].status & FIRE_STATUS_RIGHT)
                x += BLOCK_SIZE;
        }

        return;
    }
}

u8 IsTileBurned(u16 x, u16 y)
{
    u8 i;

    x = (x & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
    y = (y & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;

    for (i = 0; i < gMaxAmountOfExistingFireTiles; i++)
    {
        if (!(gFireTiles[i].parentCluster & FIRE_TILE_EXISTS))
            continue;

        if (gFireTiles[i].x == x && gFireTiles[i].y == y)
        {
            // This is very cursed, but with this I can return both the information that the tile is on fire, and the cluster id
            return TRUE + GET_LOWER_NIBBLE(gFireTiles[i].parentCluster);
        }
    }

    return FALSE;
}

void PutOutFire(u8 cluster)
{
    struct FireCluster* fire;
    cluster--;

    fire = &gFireClusters[cluster];
    fire->status = 0;
}

void UpdateFire(void)
{
    u8 i;
    struct FireCluster* fire;

    if (gGameMode.main == GM_IN_GAME)
        FireUpdateAnimation();

    fire = gFireClusters;
    for (i = 0; i < gMaxAmountOfExistingFire; i++, fire++)
    {
        if (!(fire->status & FIRE_STATUS_EXISTS))
            continue;

        gCurrentFire.status = fire->status;
        gCurrentFire.x = fire->x;
        gCurrentFire.y = fire->y;
        gCurrentFire.spreadTimer = fire->spreadTimer;
        gCurrentFire.length = fire->length;

        gCurrentClusterId = i;

        if (!(fire->status & FIRE_STATUS_LOCKED) && gGameMode.main == GM_IN_GAME)
        {
            if (!(gCurrentFire.status & FIRE_DIRECTION_MASK))
                FindFireDirection();
    
            PropagateFire();
        }            

        FireDraw();

        fire->status = gCurrentFire.status;
        fire->x = gCurrentFire.x;
        fire->y = gCurrentFire.y;
        fire->spreadTimer = gCurrentFire.spreadTimer;
        fire->length = gCurrentFire.length;
    }
}

const u8 sFireGraphics[] = {
    5,

    0x40, 0x10, 0x10, 0xA0, 0xA0, 0x50, 0x60, 0x18, 0x18, 0x80, 0x50, 0x80, 0x80, 0x88, 0x70, 0x70,
    0x08, 0x50, 0x36, 0x2A, 0x38, 0x44, 0x54, 0x28, 0x6A, 0x84, 0x02, 0x46, 0x44, 0x6C, 0x38, 0x38,
    0x00, 0x05, 0x04, 0x10, 0x0A, 0x01, 0x0D, 0x12, 0x10, 0x02, 0x00, 0x21, 0x00, 0x36, 0x0C, 0x0C,
    0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x09, 0x09, 0x00, 0x00, 0x0A, 0x01, 0x11, 0x0E, 0x0E,
    0x00, 0x00, 0x40, 0x00, 0x40, 0xA0, 0x20, 0x90, 0x90, 0x08, 0x00, 0x88, 0x80, 0x90, 0xE0, 0xE0,
};

static const u8 sFireAnim_Idle_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 0, 0,
};

static const u8 sFireAnim_Idle_Frame1[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 1, 0,
};

static const u8 sFireAnim_Idle_Frame2[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 2, 0,
};

static const u8 sFireAnim_Idle_Frame3[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 1, 0,
};

static const u8 sFireAnim_Idle_Frame4[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 4, 0,
};

static const u8 sFireAnim_Idle_Frame5[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 3, 0,
};

static const u8 sFireAnim_Idle_Frame6[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 2, 0,
};

static const u8 sFireAnim_Idle_Frame7[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(-4), 1, 0,
};

const struct AnimData sFireAnim_Idle[] = {
    [0] = {
        .oamPointer = sFireAnim_Idle_Frame0,
        .duration = 20,
    },
    [1] = {
        .oamPointer = sFireAnim_Idle_Frame1,
        .duration = 20,
    },
    [2] = {
        .oamPointer = sFireAnim_Idle_Frame2,
        .duration = 20,
    },
    [3] = {
        .oamPointer = sFireAnim_Idle_Frame3,
        .duration = 20,
    },
    [4] = {
        .oamPointer = sFireAnim_Idle_Frame4,
        .duration = 20,
    },
    [5] = {
        .oamPointer = sFireAnim_Idle_Frame5,
        .duration = 20,
    },
    [6] = {
        .oamPointer = sFireAnim_Idle_Frame6,
        .duration = 20,
    },
    [7] = {
        .oamPointer = sFireAnim_Idle_Frame7,
        .duration = 20,
    },
    [8] = SPRITE_ANIM_TERMINATOR
};
