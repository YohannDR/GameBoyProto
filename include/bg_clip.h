#ifndef BG_CLIP_H
#define BG_CLIP_H

#include "types.h"
#include "macros.h"

enum ClipdataValue {
    CLIPDATA_AIR,
    CLIPDATA_SOLID,
    CLIPDATA_LADDER,
    CLIPDATA_LADDER_TOP,
    CLIPDATA_HALF_TOP,
    CLIPDATA_HALF_BOTTOM,

    CLIPDATA_END
};

enum ClipdataCollision {
    COLLISION_AIR,
    COLLISION_SOLID,
    COLLISION_HALF_TOP,
    COLLISION_HALF_BOTTOM,
};

enum ClipdataBehavior {
    CLIP_BEHAVIOR_AIR,
    CLIP_BEHAVIOR_LADDER,
};

#define DEFAULT_AIR_TILE (0)
#define DEFAULT_SOLID_TILE (1)

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
    // The solidity of the block
    u8 solidity;
    // The behavior of the block
    u8 behavior;
};

extern struct CollisionInfo gCollisionInfo;
extern const u8* gCurrentCollisionTable;

/**
 * @brief Gets information about a clipdata value at a given position, result is in @c gCollisionInfo
 * 
 * @param x X
 * @param y Y
 */
void GetClipdataValue(u16 x, u16 y);

/**
 * @brief Sets a background tilemap value at a given position (in sub-pixels)
 * 
 * @param x X, sub-pixel
 * @param y Y, sub-pixel
 * @param value Background tile value
 */
void SetBgValueSubPixel(u16 x, u16 y, u8 value);

/**
 * @brief Sets a background tilemap value at a given position (in tiles)
 * 
 * @param x X, tiles
 * @param y Y, tiles
 * @param value Background tile value
 */
void SetBgValueTile(u8 x, u8 y, u8 value);

/**
 * @brief Applies the background value changes during v-blank
 * 
 */
void ApplyBgChanges(void);

#define GET_CLIPDATA_SOLIDITY(x, y) ((GetClipdataValue((x), (y)), gCollisionInfo.solidity))
#define GET_CLIPDATA_BEHAVIOR(x, y) ((GetClipdataValue((x), (y)), gCollisionInfo.behavior))

#endif /* BG_CLIP_H */
