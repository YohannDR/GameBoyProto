#include "bg_clip.h"
#include "macros.h"

#include "gb/memory.h"

#include "math.h"
#include "room.h"
#include "bg.h"

#define MAKE_CLIPDATA_PROP(solidity, behavior) ((solidity) | ((behavior) << 4))

struct BgTileChange {
    u8 x;
    u8 y;
    u8 newTile;
};

static struct BgTileChange gBgTileChanges[10];
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

    x -= gRoomOriginX;
    y -= gRoomOriginY;

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

void SetBgValue(u16 x, u16 y, u8 value)
{
    struct BgTileChange* ptr;

    if (gBgTileChangeSlot >= ARRAY_SIZE(gBgTileChanges))
        return;

    #ifdef HACKY_OPTIMIZATIONS
    ptr = HACKY_ARRAY_INDEXING(gBgTileChanges, gBgTileChangeSlot, struct BgTileChange);
    #else
    ptr = &gBgTileChanges[gBgTileChangeSlot];
    #endif
    ptr->x = x / 32 - 1;
    ptr->y = y / 32 - 2;
    ptr->newTile = value;
    gBgTileChangeSlot++;
}

void DrawNumber(u16 x, u16 y, u8 number)
{
    const u8 baseTile = 0x90;

    if (number <= 9)
    {
        SetBgValue(x, y, baseTile + number);
    }
}

void ApplyBgChanges(void)
{
    u8 i;
    u8* addr;
    u8 x;
    u8 y;
    u16 pos;
    struct BgTileChange* change;

    addr = (u8*)(VRAM_BASE + 0x1800);

    for (i = 0; i < gBgTileChangeSlot; i++)
    {
#ifdef HACKY_OPTIMIZATIONS
        change = HACKY_ARRAY_INDEXING(gBgTileChanges, i, struct BgTileChange);
#else
        change = &gBgTileChanges[i];
#endif

        y = change->y;
        x = change->x;
        pos = y * 32 + x;

        addr[pos] = change->newTile;
    }

    gBgTileChangeSlot = 0;
}
