#include "bg_clip.h"
#include "macros.h"

#include "gb/memory.h"

#include "data/level_tilemap.h"

struct BgTileChange {
    u8 x;
    u8 y;
    u8 newTile;
};

static const u8 sTilemapClipdataValues[] = {
    [0x80 - 0x80] = CLIPDATA_AIR,
    [0x81 - 0x80] = CLIPDATA_WALL,
    [0x82 - 0x80] = CLIPDATA_WALL,
    [0x83 - 0x80] = CLIPDATA_WALL,
    [0x84 - 0x80] = CLIPDATA_WALL,
    [0x85 - 0x80] = CLIPDATA_WALL,
    [0x86 - 0x80] = CLIPDATA_WALL,
    [0x87 - 0x80] = CLIPDATA_WALL,
    [0x88 - 0x80] = CLIPDATA_LEFT_BRICK,
    [0x89 - 0x80] = CLIPDATA_RIGHT_BRICK
};

static u8 gClipdata[LEVEL01_TILEMAP_WIDTH * LEVEL01_TILEMAP_HEIGHT];
static struct BgTileChange gBgTileChanges[10];
static u8 gBgTileChangeSlot;

void LoadClipdata(void)
{
    u16 i;

    for (i = 0; i < ARRAY_SIZE(sLevelTilemap); i++)
    {
        gClipdata[i] = sTilemapClipdataValues[sLevelTilemap[i] - 0x80];
    }
}

u8 GetClipdataValue(u8 x, u8 y)
{
    return gClipdata[y * LEVEL01_TILEMAP_WIDTH + x];
}

void SetClipdataValue(u8 x, u8 y, u8 value)
{
    gClipdata[y * LEVEL01_TILEMAP_WIDTH + x] = value;
}

void SetBgValue(u8 x, u8 y, u8 value)
{
    if (gBgTileChangeSlot >= ARRAY_SIZE(gBgTileChanges))
        return;

    gBgTileChanges[gBgTileChangeSlot].x = x;
    gBgTileChanges[gBgTileChangeSlot].y = y;
    gBgTileChanges[gBgTileChangeSlot].newTile = value;
    gBgTileChangeSlot++;
}

void DrawNumber(u8 x, u8 y, u8 number)
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
