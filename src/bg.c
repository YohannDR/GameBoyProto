#include "bg.h"

#include "gb/memory.h"

#include "door.h"
#include "macros.h"
#include "scroll.h"

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;

struct TilemapInfo gTilemap;
u8 gTilemapUpdateDirection;
u8 gTilemapUpdateBuffer[MAX(SCREEN_SIZE_X_BLOCK, SCREEN_SIZE_Y_BLOCK)];

#define DEFAULT_LOAD_X 22
#define DEFAULT_LOAD_Y 20

void LoadTilemap(const u8* tilemap)
{
    u8 i;
    u8 j;
    u8* addr;

    // A tilemap should always start with its width and height, then the data
    gTilemap.width = *tilemap++;
    gTilemap.height = *tilemap++;
    gTilemap.tilemap = tilemap;

    tilemap = &tilemap[gBackgroundInfo.blockY * gTilemap.width + gBackgroundInfo.blockX];

    addr = (u8*)(VRAM_BASE + 0x1800);

    for (i = 0; i < DEFAULT_LOAD_Y; i++)
    {
        for (j = 0; j < DEFAULT_LOAD_X; j++)
        {
            *addr++ = *tilemap++;
        }

        addr += 32 - DEFAULT_LOAD_X;
        tilemap += gTilemap.width - DEFAULT_LOAD_X;
    }
}

void BindTilemap(const u8* tilemap)
{
    gTilemap.width = *tilemap++;
    gTilemap.height = *tilemap++;
    gTilemap.tilemap = tilemap;
}

void SetupTilemapUpdate(u8 direction)
{
    u8 offset;
    u8 i;
    const u8* src;
    u8* dst;

    gTilemapUpdateDirection = direction;

    if (gTilemapUpdateDirection == TILEMAP_UPDATE_LEFT || gTilemapUpdateDirection == TILEMAP_UPDATE_RIGHT)
    {
        if (gTilemapUpdateDirection == TILEMAP_UPDATE_LEFT)
            offset = gCamera.left;
        else
            offset = gCamera.right;

        src = &gTilemap.tilemap[offset];
        dst = gTilemapUpdateBuffer;
        for (i = 0; i < SCREEN_SIZE_Y_BLOCK; i++)
        {
            *dst++ = *src;
            src += gTilemap.width;
        }
    }
}
