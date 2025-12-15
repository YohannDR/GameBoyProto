#ifndef GFX_LOADER_H
#define GFX_LOADER_H

#include "types.h"

/**
 * @brief Information about the multi frame graphics loading process
 * 
 */
struct GraphicsLoaderInfo {
    // The state of the loader
    u8 state;
    // The current VRAM address we're writting to
    u8* vramAddr;
    // The current ROM gfx address we're reading from
    const u8* gfxAddr;
    // The amount of tiles we have loaded so far
    u8 nbrTilesLoaded;
    // The number of tiles to load for the current graphics
    u8 nbrTilesToLoad;
    // The amount of bytes we actually buffered
    u8 nbrBytesBuffered;
};

enum GraphicsLoaderInfoState {
    GRAPHICS_LOADER_OFF,
    GRAPHICS_LOADER_ON,
    GRAPHICS_LOADER_LAST_UPDATE,

    GRAPHICS_LOADER_TILEMAP = 1u << 6,
    GRAPHICS_LOADER_TILESET = 1u << 7
};

/**
 * @brief Graphics loader info
 * 
 */
extern struct GraphicsLoaderInfo gGraphicsLoaderInfo;

/**
 * @brief The buffer that holds the graphics we'll send to VRAM
 * 
 */
extern u8 gGraphicsLoaderBuffer[16 * 4];

#endif /* GFX_LOADER_H */
