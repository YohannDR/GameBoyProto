#include "bg.h"

#include "macros.h"
#include "gb/memory.h"

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

    // Set default screen boundaries
    gTilemap.left = gBackgroundInfo.blockX;
    gTilemap.right = gBackgroundInfo.blockX + DEFAULT_LOAD_X;
    gTilemap.top = gBackgroundInfo.blockY;
    gTilemap.bottom = gBackgroundInfo.blockY + DEFAULT_LOAD_Y;

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

void CheckForTilemapUpdate(void)
{
    u8 left;
    u8 right;

    // Update block coordinates
    gBackgroundInfo.blockX = PIXEL_TO_BLOCK(gBackgroundInfo.x);
    gBackgroundInfo.blockY = PIXEL_TO_BLOCK(gBackgroundInfo.y);

    gTilemapUpdateDirection = TILEMAP_UPDATE_NONE;

    // Get crrent boundaries
    left = gBackgroundInfo.blockX;
    right = gBackgroundInfo.blockX + SCREEN_SIZE_X_BLOCK;

    // Check if any has been crossed, and flag for update
    if (right == gTilemap.right - 1)
    {
        gTilemapUpdateDirection = TILEMAP_UPDATE_RIGHT;
        gTilemap.right++;
        gTilemap.left++;
        return;
    }

    if (left == gTilemap.left - 1)
    {
        gTilemapUpdateDirection = TILEMAP_UPDATE_LEFT;
        gTilemap.right--;
        gTilemap.left--;
        return;
    }
}
