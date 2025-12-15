#ifndef BG_H
#define BG_H

#include "types.h"
#include "macros.h"

#define DEFAULT_LOAD_Y 32
#define DEFAULT_LOAD_X 32

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

extern u16 gBackgroundX;
extern u16 gBackgroundY;
extern u8 gWindowX;
extern u8 gWindowY;

extern struct TilemapInfo gTilemap;

extern u8 gDecompressedTilemap[DEFAULT_LOAD_Y * DEFAULT_LOAD_X];

/**
 * @brief Loads a specified tilemap
 * 
 * @param tilemap Tilemap
 */
void LoadTilemap(const u8* tilemap);

void UpdateTilemapLoading(void);

#endif /* BG_H */
