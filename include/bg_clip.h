#ifndef BG_CLIP_H
#define BG_CLIP_H

#include "types.h"
#include "macros.h"

enum ClipdataValue {
    CLIPDATA_AIR,
    CLIPDATA_SOLID
};

/**
 * @brief Contains information about the last block collision checked
 * 
 */
struct CollisionInfo {
    // The Y coordinate of the top of the block
    u16 top;
    // The Y coordinate of the bottom of the block
    u16 bottom;
    // The X coordinate of the left of the block
    u16 left;
    // The X coordinate of the right of the block
    u16 right;
};

extern struct CollisionInfo gCollisionInfo;

void LoadClipdata(const u8* clipdata);

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
