#include "bg.h"

#include "gb/io.h"
#include "gb/display.h"
#include "gb/memory.h"

#include "io.h"
#include "door.h"
#include "gfx_loader.h"
#include "macros.h"
#include "scroll.h"

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;
u16 gBackgroundX;
u16 gBackgroundY;

struct TilemapInfo gTilemap;

u8 gDecompressedTilemap[DEFAULT_LOAD_Y * DEFAULT_LOAD_X];
u8 gTilemapVramCopy[DEFAULT_LOAD_Y * DEFAULT_LOAD_X];

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

    tilemap = gTilemap.tilemap;

    addr = gTilemapVramCopy;

    if (Read8(REG_LCDC) == 0)
    {
        // Directly write to vram if we can
        addr = (u8*)(VRAM_BASE + 0x1800);
    }

    for (i = 0; i < DEFAULT_LOAD_Y; i++)
    {
        for (j = 0; j < DEFAULT_LOAD_X; j++)
            *addr++ = *tilemap++;

        addr += 32 - DEFAULT_LOAD_X;
        tilemap += gTilemap.width - DEFAULT_LOAD_X;
    }

    if (Read8(REG_LCDC) != 0)
    {
        gGraphicsLoaderInfo.state = GRAPHICS_LOADER_ON | GRAPHICS_LOADER_TILEMAP;
        gGraphicsLoaderInfo.vramAddr = (u8*)(VRAM_BASE + 0x1800 - ARRAY_SIZE(gGraphicsLoaderBuffer));
        gGraphicsLoaderInfo.gfxAddr = gTilemapVramCopy;
        gGraphicsLoaderInfo.nbrTilesLoaded = 0;
        gGraphicsLoaderInfo.nbrTilesToLoad = 64;
        gGraphicsLoaderInfo.nbrBytesBuffered = 0;
    }
}
