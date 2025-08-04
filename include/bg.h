#ifndef BG_H
#define BG_H

#include "types.h"
#include "macros.h"

/**
 * @brief Holds information about the current tilemap
 * 
 */
struct TilemapInfo {
    // Pointer to the current tilemap
    const u8* tilemap;
    // Width of the tilemap
    u8 width;
    // Height of the tilemap
    u8 height;

    // Reserved field, used in an assembly function
    u8 reserved;
};

/**
 * @brief Determines in which direction the tilemap should be updated
 * 
 */
enum TilemapUpdateDirection {
    TILEMAP_UPDATE_NONE,
    TILEMAP_UPDATE_LEFT,
    TILEMAP_UPDATE_RIGHT,
    TILEMAP_UPDATE_TOP,
    TILEMAP_UPDATE_BOTTOM,
};

/**
 * @brief Information about the current background scrolling
 * 
 */
struct BackgroundInfo {
    // X position
    u16 x;
    // Y position
    u16 y;
    // Block X position
    u8 blockX;
    // Block Y position
    u8 blockY;
};

extern struct BackgroundInfo gBackgroundInfo;

extern u8 gWindowX;
extern u8 gWindowY;

extern struct TilemapInfo gTilemap;

extern u8 gTilemapUpdateDirection;

/**
 * @brief Loads a specified tilemap
 * 
 * @param tilemap Tilemap
 */
void LoadTilemap(const u8* tilemap);

void LoadTilemapForTransition(const u8* tilemap);

/**
 * @brief Updates the tilemap during v-blank
 * 
 */
ASM_IMPL void TilemapUpdateVblank(void);

#endif /* BG_H */
