#ifndef BG_CLIP_H
#define BG_CLIP_H

#include "types.h"

#define OBJ_TO_TILE_X(x) ((x - 8) / 8)
#define OBJ_TO_TILE_Y(y) ((y - 16) / 8)

enum ClipdataValue {
    CLIPDATA_AIR,
    CLIPDATA_WALL,
    CLIPDATA_LEFT_BRICK,
    CLIPDATA_RIGHT_BRICK
};

/**
 * @brief Loads the clipdata
 * 
 */
void LoadClipdata(void);

/**
 * @brief Gets a clipdata value at a given position
 * 
 * @param x X
 * @param y Y
 * @return u8 Clipdata value
 */
u8 GetClipdataValue(u8 x, u8 y);

/**
 * @brief Sets a clipdata value at a given position
 * 
 * @param x X
 * @param y Y
 * @param value Clipdata value
 */
void SetClipdataValue(u8 x, u8 y, u8 value);

/**
 * @brief Sets a background tilemap value at a given position
 * 
 * @param x X
 * @param y Y
 * @param value Background tile value
 */
void SetBgValue(u8 x, u8 y, u8 value);

/**
 * @brief Draws a number of the screen
 * 
 * @param x X
 * @param y Y
 * @param number Number
 */
void DrawNumber(u8 x, u8 y, u8 number);

/**
 * @brief Applies the background value changes during v-blank
 * 
 */
void ApplyBgChanges(void);

#endif /* BG_CLIP_H */
