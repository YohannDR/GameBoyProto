#include "bg_clip.h"
#include "macros.h"

#include "gb/memory.h"

#include "bg.h"
#include "math.h"
#include "scroll.h"
#include "room.h"

#define MAKE_CLIPDATA_PROP(solidity, behavior) ((solidity) | ((behavior) << 4))

struct BgTileChange {
    u16 offset;
    u8 newTile;
};

static struct BgTileChange gBgTileChanges[20];
static u8 gBgTileChangeSlot;

struct CollisionInfo gCollisionInfo;
const u8* gCurrentCollisionTable;

static const u8 sClipdataTable[] = {
    [CLIPDATA_AIR]                  = MAKE_CLIPDATA_PROP(COLLISION_AIR, CLIP_BEHAVIOR_AIR),
    [CLIPDATA_SOLID]                = MAKE_CLIPDATA_PROP(COLLISION_SOLID, CLIP_BEHAVIOR_AIR),
    [CLIPDATA_LADDER]               = MAKE_CLIPDATA_PROP(COLLISION_AIR, CLIP_BEHAVIOR_LADDER),
    [CLIPDATA_LADDER_TOP]           = MAKE_CLIPDATA_PROP(COLLISION_SOLID, CLIP_BEHAVIOR_LADDER),
    [CLIPDATA_WATER]                = MAKE_CLIPDATA_PROP(COLLISION_AIR, CLIP_BEHAVIOR_WATER),
    [CLIPDATA_ACCESS_CARD_READER]   = MAKE_CLIPDATA_PROP(COLLISION_AIR, CLIP_BEHAVIOR_ACCESS_CARD_READER),
    [CLIPDATA_RECEPTACLE]           = MAKE_CLIPDATA_PROP(COLLISION_SOLID, CLIP_BEHAVIOR_RECEPTACLE),
    [CLIPDATA_INFLAMMABLE]          = MAKE_CLIPDATA_PROP(COLLISION_SOLID, CLIP_BEHAVIOR_INFLAMMABLE),
};

void GetClipdataValue(u16 x, u16 y)
{
    u8 clipdata;
    u8 subPixel;
    u8 data;

    gCollisionInfo.top = y & BLOCK_POSITION_FLAG;
    gCollisionInfo.bottom = gCollisionInfo.top + BLOCK_SIZE;
    gCollisionInfo.left = x & BLOCK_POSITION_FLAG;
    gCollisionInfo.right = gCollisionInfo.left + BLOCK_SIZE;

    clipdata = gTilemap.tilemap[ComputeIndexFromSpriteCoords(y, gTilemap.width, x)];
    clipdata = gCurrentCollisionTable[clipdata];

    data = sClipdataTable[clipdata];
    gCollisionInfo.solidity = GET_LOWER_NIBBLE(data);
    gCollisionInfo.behavior = GET_UPPER_NIBBLE(data);

    if (clipdata == CLIPDATA_LADDER_TOP)
    {
        subPixel = y & SUB_PIXEL_POSITION_FLAG;

        if (subPixel >= BLOCK_SIZE / 2)
            gCollisionInfo.solidity = CLIPDATA_AIR;
        else
            gCollisionInfo.solidity = CLIPDATA_SOLID;
    }
}

void SetBgValueSubPixel(u16 x, u16 y, u8 value)
{
    SetBgValueTile(SUB_PIXEL_TO_BLOCK(x) - 1, SUB_PIXEL_TO_BLOCK(y) - 2, value);
}

void SetBgValueTile(u8 x, u8 y, u8 value)
{
    // Write the value to the tilemap
    gDecompressedTilemap[y * gTilemap.width + x] = value;

    // Handle specific case where the modified tile is currently on the screen
    if (gCamera.left < x && x < gCamera.right && gCamera.top < y && y < gCamera.bottom)
    {
        // Also setup an update to VRAM is this case
        gBgTileChanges[gBgTileChangeSlot].offset = y * 32 + x;
        gBgTileChanges[gBgTileChangeSlot].newTile = value;

        gBgTileChangeSlot++;
    }
}

void ApplyBgChanges(void)
{
    u8 i;
    u8* addr;
    struct BgTileChange* change;

    addr = (u8*)(VRAM_BASE + 0x1800);

    for (i = 0; i < gBgTileChangeSlot; i++)
    {
#ifdef HACKY_OPTIMIZATIONS
        change = HACKY_ARRAY_INDEXING(gBgTileChanges, i, struct BgTileChange);
#else
        change = &gBgTileChanges[i];
#endif

        addr[change->offset] = change->newTile;
    }

    gBgTileChangeSlot = 0;
}
