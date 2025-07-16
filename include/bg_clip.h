#ifndef BG_CLIP_H
#define BG_CLIP_H

#include "types.h"
#include "macros.h"

enum ClipdataValue {
    CLIPDATA_AIR,
    CLIPDATA_SOLID
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
u8 GetClipdataValue(u16 x, u16 y);

/**
 * @brief Sets a clipdata value at a given position
 * 
 * @param x X
 * @param y Y
 * @param value Clipdata value
 */
void SetClipdataValue(u16 x, u16 y, u8 value);

/**
 * @brief Sets a background tilemap value at a given position
 * 
 * @param x X
 * @param y Y
 * @param value Background tile value
 */
void SetBgValue(u16 x, u16 y, u8 value);

/**
 * @brief Draws a number of the screen
 * 
 * @param x X
 * @param y Y
 * @param number Number
 */
void DrawNumber(u16 x, u16 y, u8 number);

/**
 * @brief Applies the background value changes during v-blank
 * 
 */
void ApplyBgChanges(void);

#endif /* BG_CLIP_H */
