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
 * @brief This buffer holds the tiles that will be used to udpate the tilemap, it can contains either a column or a line.
 * 
 * The buffer is updated during normal frame processing and used during v-blank, this allows to alleviate the v-blank code
 * since it already receives the processed buffer and just has to perform the copy.
 * 
 * Moreover, this also allows applying modifications to the tilemap without having to fully copy it into a RAM buffer,
 * since now we can easily decide what to send to the tilemap instead of just copying it from ROM.
 * 
 */
extern u8 gTilemapUpdateBuffer[MAX(SCREEN_SIZE_X_BLOCK, SCREEN_SIZE_Y_BLOCK)];

/**
 * @brief Loads a specified tilemap
 * 
 * @param tilemap Tilemap
 */
void LoadTilemap(const u8* tilemap);

/**
 * @brief Sets up a tilemap update in the desired direction
 * 
 * @param direction Direction
 */
void SetupTilemapUpdate(u8 direction);

/**
 * @brief Updates the tilemap during v-blank
 * 
 */
ASM_IMPL void TilemapUpdateVblank(void);

#endif /* BG_H */
