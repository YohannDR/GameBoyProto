#include "bg_clip.h"
#include "macros.h"

#include "gb/memory.h"

#include "math.h"
#include "bg.h"

#include "data/level_tilemap.h"

struct BgTileChange {
    u8 x;
    u8 y;
    u8 newTile;
};

static const u8 sTilemapClipdataValues[] = {
    [0] = CLIPDATA_AIR,
    [1] = CLIPDATA_AIR,
    [2] = CLIPDATA_SOLID,
    [3] = CLIPDATA_AIR,
    [4] = CLIPDATA_AIR,
    [5] = CLIPDATA_AIR,
    [6] = CLIPDATA_AIR,
    [7] = CLIPDATA_AIR,
    [8] = CLIPDATA_AIR,
    [9] = CLIPDATA_AIR
};

u8 gClipdata[LEVEL01_TILEMAP_WIDTH * LEVEL01_TILEMAP_HEIGHT];
static struct BgTileChange gBgTileChanges[10];
static u8 gBgTileChangeSlot;

void LoadClipdata(void)
{
    u16 i;
    u16 size;

    size = gTilemap.width * gTilemap.height;

    for (i = 0; i < size; i++)
    {
        gClipdata[i] = sTilemapClipdataValues[gTilemap.tilemap[i]];
    }
}

u8 GetClipdataValue(u16 x, u16 y)
{
    return gClipdata[ComputeIndexFromSpriteCoords(y, gTilemap.width, x)];
}

void SetClipdataValue(u16 x, u16 y, u8 value)
{
    gClipdata[ComputeIndexFromSpriteCoords(y, gTilemap.width, x)] = value;
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
