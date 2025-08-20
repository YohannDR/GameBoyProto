#include "fire.h"

#include "gb/oam.h"

#include "macros.h"
#include "sprite.h"
#include "bg_clip.h"

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

#define FIRE_DIRECTION_MASK (FIRE_STATUS_UP | FIRE_STATUS_DOWN | FIRE_STATUS_LEFT | FIRE_STATUS_RIGHT)

#define FIRE_SPREADING_DURATION (CONVERT_SECONDS(4.f))
#define FIRE_MAX_LENGTH (10)

struct FireCluster {
    u8 status;
    u16 x;
    u16 y;
    u8 spreadTimer;
    u8 length;
};

struct FireCluster gFireClusters[4];
static struct FireCluster gCurrentFire;
static u8 gMaxAmountOfExistingFire;

static void FindFireDirection(void)
{
    u16 x;
    u16 y;

    x = gCurrentFire.x;
    y = gCurrentFire.y;

    if (GET_CLIPDATA_BEHAVIOR(x, y - BLOCK_SIZE) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        gCurrentFire.status |= FIRE_STATUS_UP;
        return;
    }

    if (GET_CLIPDATA_BEHAVIOR(x, y + BLOCK_SIZE) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        gCurrentFire.status |= FIRE_STATUS_DOWN;
        return;
    }

    if (GET_CLIPDATA_BEHAVIOR(x - BLOCK_SIZE, y) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        gCurrentFire.status |= FIRE_STATUS_LEFT;
        return;
    }

    if (GET_CLIPDATA_BEHAVIOR(x + BLOCK_SIZE, y) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        gCurrentFire.status |= FIRE_STATUS_RIGHT;
        return;
    }

    gCurrentFire.status = FIRE_STATUS_LOCKED;
}

void PropagateFire(void)
{
    u16 x;
    u16 y;
    u16 offset;

    gCurrentFire.spreadTimer += DELTA_TIME;

    if (gCurrentFire.spreadTimer != FIRE_SPREADING_DURATION)
        return;

    gCurrentFire.spreadTimer = 0;

    x = gCurrentFire.x;
    y = gCurrentFire.y;
    offset = (gCurrentFire.length + 1) * BLOCK_SIZE;

    if (gCurrentFire.status & FIRE_STATUS_UP)
        y -= offset;
    else if (gCurrentFire.status & FIRE_STATUS_DOWN)
        y += offset;
    else if (gCurrentFire.status & FIRE_STATUS_LEFT)
        x -= offset;
    else if (gCurrentFire.status & FIRE_STATUS_RIGHT)
        x += offset;
    
    if (GET_CLIPDATA_BEHAVIOR(x, y) != CLIP_BEHAVIOR_INFLAMMABLE)
    {
        gCurrentFire.status |= FIRE_STATUS_LOCKED;
        return;
    }

    gCurrentFire.length++;
    if (gCurrentFire.length == FIRE_MAX_LENGTH)
        gCurrentFire.status |= FIRE_STATUS_LOCKED;
}

static void FireDraw(void)
{
}

void StartFire(u16 x, u16 y)
{
    u8 i;

    for (i = 0; i < ARRAY_SIZE(gFireClusters); i++)
    {
        if (gFireClusters[i].status & FIRE_STATUS_EXISTS)
            continue;

        gFireClusters[i].status = FIRE_STATUS_EXISTS;
        gFireClusters[i].x = (x & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
        gFireClusters[i].y = (y & BLOCK_POSITION_FLAG) + HALF_BLOCK_SIZE;
        gFireClusters[i].spreadTimer = 0;
        
        if (i >= gMaxAmountOfExistingFire)
            gMaxAmountOfExistingFire = i + 1;

        return;
    }
}

void UpdateFire(void)
{
    u8 i;
    struct FireCluster* fire;

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

        if (!(fire->status & FIRE_STATUS_LOCKED))
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
