#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"

/**
 * @brief Represents a sprite, or an object in the engine
 * 
 */
struct Sprite {
    // Holds a bunch of flags about the sprite
    u8 status;
    // X position
    u16 x;
    // Y position
    u16 y;
    // Determines the behavior of the sprite within its AI
    u8 pose;
    // The type of the sprite, each sprite a unique ID
    u8 type;

    // Current animation frame
    u8 currentAnimFrame;
    // Timer for the current animation frame
    u8 animTimer;

    // Work variable, essentially free space
    u8 work1;
    // Work variable, essentially free space
    u8 work2;
    // Work variable, essentially free space
    u8 work3;
    // Work variable, essentially free space
    u8 work4;
};

typedef void (*SpriteFunc_T)(struct Sprite*);

enum SpriteType {
    STYPE_PLAYER,

    STYPE_END
};

// Indicates that the sprite exists, clearing this flag "kills" the sprite
#define SPRITE_STATUS_EXISTS    (1u << 0)
// READ ONLY Whether the sprite is on screen
#define SPRITE_STATUS_ON_SCREEN (1u << 1)
// Whether the sprite should be drawn or not
#define SPRITE_STATUS_NOT_DRAWN (1u << 2)
// Whether the sprite is X flipped
#define SPRITE_STATUS_X_FLIP    (1u << 3)
// Whether the sprite is Y flipped
#define SPRITE_STATUS_Y_FLIP    (1u << 4)

extern struct Sprite gSpriteData[20];

/**
 * @brief Spawns a new sprite
 * 
 * @param x X position
 * @param y Y position
 * @param type Sprite type
 * @return u8 Sprite slot if it could spawn, UCHAR_MAX otherwise
 */
u8 SpawnSprite(u8 x, u8 y, u8 type);

/**
 * @brief Updates the sprites
 * 
 */
void UpdateSprites(void);

/**
 * @brief Draws all the sprites
 * 
 */
void DrawSprites(void);

#endif /* SPRITE_H */
