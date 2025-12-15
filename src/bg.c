#include "bg.h"

#include "gb/memory.h"

#include "door.h"
#include "macros.h"
#include "scroll.h"

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;
u16 gBackgroundX;
u16 gBackgroundY;

struct TilemapInfo gTilemap;

u8 gDecompressedTilemap[SCREEN_SIZE_X_BLOCK * SCREEN_SIZE_Y_BLOCK * 4];

#define DEFAULT_LOAD_Y 32
#define DEFAULT_LOAD_X 32

static void DecompressTilemap(const u8* tilemap)
{
    u8 amount;
    u8 value;
    u8* dst;

    dst = gDecompressedTilemap;
    for (;;)
    {
        amount = *tilemap++;

        if (amount == 0)
            break;

        value = *tilemap++;

        while (amount--)
            *dst++ = value;
    }
}

void LoadTilemap(const u8* tilemap)
{
    u8 i;
    u8 j;
    u8* addr;

    // A tilemap should always start with its width and height, then the data
    gTilemap.width = *tilemap++;
    gTilemap.height = *tilemap++;
    DecompressTilemap(tilemap);
    gTilemap.tilemap = gDecompressedTilemap;

    tilemap = &gTilemap.tilemap[gBackgroundInfo.blockY * gTilemap.width + gBackgroundInfo.blockX];

    addr = (u8*)(VRAM_BASE + 0x1800);

    for (i = 0; i < DEFAULT_LOAD_Y; i++)
    {
        for (j = 0; j < DEFAULT_LOAD_X; j++)
            *addr++ = *tilemap++;

        addr += 32 - DEFAULT_LOAD_X;
        tilemap += gTilemap.width - DEFAULT_LOAD_X;
    }
}