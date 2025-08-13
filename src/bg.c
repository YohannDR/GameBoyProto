#include "bg.h"

#include "gb/memory.h"

#include "door.h"
#include "macros.h"
#include "scroll.h"

#define TILEMAP_UPDATE_X_OVERDRAW 4
#define TILEMAP_UPDATE_Y_OVERDRAW 4

struct BackgroundInfo gBackgroundInfo;

u8 gWindowX;
u8 gWindowY;

struct TilemapInfo gTilemap;

u8 gTilemapUpdateDirectionX;

/**
 * @brief These buffers holds the tiles that will be used to udpate the tilemap.
 * There's one buffer for a line update (Y) and one for a column update (X).
 * 
 * The buffer is updated during normal frame processing and used during v-blank, this allows to alleviate the v-blank code
 * since it already receives the processed buffer and just has to perform the copy.
 * 
 * Moreover, this also allows applying modifications to the tilemap without having to fully copy it into a RAM buffer,
 * since now we can easily decide what to send to the tilemap instead of just copying it from ROM.
 * 
 */
u8 gTilemapUpdateBufferX[SCREEN_SIZE_Y_BLOCK + TILEMAP_UPDATE_X_OVERDRAW];

u8 gTilemapUpdateDirectionY;
u8 gTilemapUpdateBufferY[SCREEN_SIZE_X_BLOCK + TILEMAP_UPDATE_Y_OVERDRAW];

#define DEFAULT_LOAD_Y 32
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

void SetupTilemapUpdateX(u8 direction)
{
    u8 offset;
    u8 i;
    const u8* src;
    u8* dst;

    gTilemapUpdateDirectionX = direction;
    gTilemap.tilemapUpdateVramAddrVertical = VRAM_BASE + 0x1800;

    if (gTilemapUpdateDirectionX == TILEMAP_UPDATE_LEFT)
    {
        offset = gCamera.left;
        gTilemap.tilemapUpdateVramAddrVertical += (u8)(gBackgroundInfo.blockX - 1) % 32;
    }
    else
    {
        offset = gCamera.right;
        gTilemap.tilemapUpdateVramAddrVertical += (u8)(gBackgroundInfo.blockX + 20) % 32;
    }

    gTilemap.tilemapUpdateVramAddrVertical += ((u8)(gBackgroundInfo.blockY - TILEMAP_UPDATE_X_OVERDRAW / 2) % 32) * 32;
    gTilemap.tilemapUpdateVramAddrVertical &= 0xFBFF;

    src = &gTilemap.tilemap[offset + (gBackgroundInfo.tilemapAnchorY - TILEMAP_UPDATE_X_OVERDRAW / 2) * gTilemap.width];
    dst = gTilemapUpdateBufferX;
    for (i = 0; i < ARRAY_SIZE(gTilemapUpdateBufferX); i++)
    {
        *dst++ = *src;
        src += gTilemap.width;
    }
}

void SetupTilemapUpdateY(u8 direction)
{
    u8 offset;
    u8 i;
    const u8* src;
    u8* dst;

    gTilemapUpdateDirectionY = direction;
    gTilemap.tilemapUpdateVramAddrHorizontal = VRAM_BASE + 0x1800;

    if (gTilemapUpdateDirectionY == TILEMAP_UPDATE_TOP)
    {
        offset = gCamera.top;
        gTilemap.tilemapUpdateVramAddrHorizontal += ((u8)(gBackgroundInfo.blockY - 1) % 32) * 32;
    }
    else
    {
        offset = gCamera.bottom;
        gTilemap.tilemapUpdateVramAddrHorizontal += ((u8)(gBackgroundInfo.blockY + 18) % 32) * 32;
    }

    gTilemap.tilemapUpdateVramAddrHorizontal += (u8)(gBackgroundInfo.blockX - TILEMAP_UPDATE_Y_OVERDRAW / 2) % 32;
    gTilemap.tilemapUpdateVramAddrVertical &= 0xFBFF;

    src = &gTilemap.tilemap[gBackgroundInfo.tilemapAnchorX - TILEMAP_UPDATE_Y_OVERDRAW / 2 + offset * gTilemap.width];
    dst = gTilemapUpdateBufferY;
    for (i = 0; i < ARRAY_SIZE(gTilemapUpdateBufferY); i++)
        *dst++ = *src++;
}
