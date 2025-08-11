#include "bg.h"

#include "gb/memory.h"

#include "door.h"
#include "macros.h"
#include "scroll.h"

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;

struct TilemapInfo gTilemap;
u8 gTilemapUpdateVerticalDirection;
u8 gTilemapUpdateHorizontalDirection;
u8 gTilemapUpdateBufferHorizontal[SCREEN_SIZE_X_BLOCK + 3];
u8 gTilemapUpdateBufferVertical[SCREEN_SIZE_Y_BLOCK + 2];

#define DEFAULT_LOAD_X 22

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

    for (i = 0; i < 32; i++)
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

    if (direction == TILEMAP_UPDATE_LEFT || direction == TILEMAP_UPDATE_RIGHT)
    {
        gTilemapUpdateVerticalDirection = direction;
        gTilemap.tilemapUpdateVramAddrVertical = VRAM_BASE + 0x1800;

        if (gTilemapUpdateVerticalDirection == TILEMAP_UPDATE_LEFT)
        {
            offset = gCamera.left;
            gTilemap.tilemapUpdateVramAddrVertical += (u8)(gBackgroundInfo.blockX - 1) % 32;
        }
        else
        {
            offset = gCamera.right;
            gTilemap.tilemapUpdateVramAddrVertical += (u8)(gBackgroundInfo.blockX + 20) % 32;
        }

        gTilemap.tilemapUpdateVramAddrVertical += ((u8)(gBackgroundInfo.blockY - 1) % 32) * 32;
        gTilemap.tilemapUpdateVramAddrVertical &= 0xFBFF;

        src = &gTilemap.tilemap[offset + (gBackgroundInfo.tilemapAnchorY - 1) * gTilemap.width];
        dst = gTilemapUpdateBufferVertical;
        for (i = 0; i < ARRAY_SIZE(gTilemapUpdateBufferVertical); i++)
        {
            *dst++ = *src;
            src += gTilemap.width;
        }
    }
    else if (direction == TILEMAP_UPDATE_TOP || direction == TILEMAP_UPDATE_BOTTOM)
    {
        gTilemapUpdateHorizontalDirection = direction;
        gTilemap.tilemapUpdateVramAddrHorizontal = VRAM_BASE + 0x1800;

        if (gTilemapUpdateHorizontalDirection == TILEMAP_UPDATE_TOP)
        {
            offset = gCamera.top;
            gTilemap.tilemapUpdateVramAddrHorizontal += ((u8)(gBackgroundInfo.blockY - 1) % 32) * 32;
        }
        else
        {
            offset = gCamera.bottom;
            gTilemap.tilemapUpdateVramAddrHorizontal += ((u8)(gBackgroundInfo.blockY + 18) % 32) * 32;
        }

        gTilemap.tilemapUpdateVramAddrHorizontal += (u8)(gBackgroundInfo.blockX - 1) % 32;
        gTilemap.tilemapUpdateVramAddrVertical &= 0xFBFF;

        src = &gTilemap.tilemap[gBackgroundInfo.tilemapAnchorX - 1 + offset * gTilemap.width];
        dst = gTilemapUpdateBufferHorizontal;
        for (i = 0; i < ARRAY_SIZE(gTilemapUpdateBufferHorizontal); i++)
            *dst++ = *src++;
    }
}
