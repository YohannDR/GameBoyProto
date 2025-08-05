#include "bg_clip.h"
#include "macros.h"

#include "gb/memory.h"

#include "math.h"
#include "bg.h"

struct BgTileChange {
    u8 x;
    u8 y;
    u8 newTile;
};

static struct BgTileChange gBgTileChanges[10];
static u8 gBgTileChangeSlot;

struct CollisionInfo gCollisionInfo;
const u8* gCurrentCollisionTable;

u8 GetClipdataValue(u16 x, u16 y)
{
    u8 clipdata;

    gCollisionInfo.top = y & BLOCK_POSITION_FLAG;
    gCollisionInfo.bottom = gCollisionInfo.top + BLOCK_SIZE;
    gCollisionInfo.left = x & BLOCK_POSITION_FLAG;
    gCollisionInfo.right = gCollisionInfo.left + BLOCK_SIZE;

    clipdata = gTilemap.tilemap[ComputeIndexFromSpriteCoords(y, gTilemap.width, x)];
    clipdata = gCurrentCollisionTable[clipdata];

    return clipdata;
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
