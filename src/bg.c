#include "bg.h"

#include "gb/memory.h"

#include "door.h"
#include "macros.h"

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;

struct TilemapInfo gTilemap;
u8 gTilemapUpdateDirection;

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
