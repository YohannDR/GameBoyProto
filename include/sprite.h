#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"

/**
 * @brief Animation data about a sprite
 * 
 * It consists of a pointer to raw oam that makes up the sprite's part, and the duration of the frame
 * The raw oam always starts with the amount of parts, or the amount of oam, and is then followed by
 * entries of raw, Game Boy, oam, whose amount is determined by the amount of parts.
 * 
 * e.g. it can look like this :
 * 
 * 1 // Number of parts
 * 
 * 0, 0, 4, 0 // Raw game boy oam (y, x, tile, attributes)
 * 
 * or like this :
 * 
 * 2 // Number of parts
 * 
 * 0, 0, 4, 0 // Raw game boy oam (y, x, tile, attributes), first entry
 * 
 * 0, 8, 5, 0 // Raw game boy oam (y, x, tile, attributes), second entry
 * 
 */
struct AnimData {
    // Pointer to raw oam
    const u8* oamPointer;
    // Duration of the frame
    u8 duration;
};

/**
 * @brief Represents a sprite, or an object in the engine
 * 
 */
struct Sprite {
    // Holds a bunch of flags about the sprite
    u8 status;
    // X position
    u8 x;
    // Y position
    u8 y;
    // The type of the sprite, each sprite a unique ID
    u8 type;
    // The slot in the global sprite array of this sprite
    u8 ramSlot;
    // Part of the sprite, can be used to choose the behavior of a sprite
    u8 part;
    // Determines the behavior of the sprite within its AI
    u8 pose;

    // Current animation frame
    u8 currentAnimFrame;
    // Timer for the current animation frame
    u8 animTimer;
    // Pointer to the current animation
    const struct AnimData* animPointer;
    // Holds a bunch of flags about the sprite
    u8 properties;

    // Work variable, essentially free space
    u8 work1;
    // Work variable, essentially free space
    u8 work2;
    // Work variable, essentially free space
    u8 work3;
    // Work variable, essentially free space
    u8 work4;
};

/**
 * @brief The different types of sprites
 * 
 */
enum SpriteType {
    STYPE_PADDLE,
    STYPE_BALL,
    STYPE_PARTICLE,

    STYPE_END
};

// Indicates that the sprite exists, clearing this flag "kills" the sprite
#define SPRITE_STATUS_EXISTS        (1u << 0)
// READ ONLY Whether the sprite is on screen
#define SPRITE_STATUS_ON_SCREEN     (1u << 1)
// Whether the sprite should be drawn or not
#define SPRITE_STATUS_NOT_DRAWN     (1u << 2)
// READ ONLY Whether the sprite has finished its current animation
#define SPRITE_STATUS_ANIM_ENDED    (1u << 3)
// Whether the sprite is moving right
#define SPRITE_STATUS_MOVING_RIGHT  (1u << 4)
// Whether the sprite is moving down
#define SPRITE_STATUS_MOVING_DOWN   (1u << 5)

// Whether the sprite uses object palette 1
#define SPRITE_PROPERTY_OBP1        (1u << 0)
// Whether the sprite is X flipped
#define SPRITE_PROPERTY_X_FLIP      (1u << 1)
// Whether the sprite is Y flipped
#define SPRITE_PROPERTY_Y_FLIP      (1u << 2)
// Whether the sprite has low priority
#define SPRITE_PROPERTY_LOW_PRIO    (1u << 3)

#define SPRITE_PROPERTY_GFX (SPRITE_PROPERTY_OBP1 | SPRITE_PROPERTY_X_FLIP | SPRITE_PROPERTY_Y_FLIP | SPRITE_PROPERTY_LOW_PRIO)

/**
 * @brief Determines the size of the oam data for a sprite @c AnimData
 * 
 * @param partCount Amount of parts
 */
#define OAM_DATA_SIZE(partCount) (1 + (partCount * 4))

// Terminator value for a sprite animation data
#define SPRITE_ANIM_TERMINATOR  \
{                               \
    .oamPointer = NULL,         \
    .duration = 0               \
}

// Global array that holds all the sprites
extern struct Sprite gSpriteData[20];
// Holds the current sprite being processed
extern struct Sprite gCurrentSprite;

/**
 * @brief Spawns a new sprite
 * 
 * @param x X position
 * @param y Y position
 * @param type Sprite type
 * @param part Sprite part, used to determine the specific behavior of a sprite
 * @return u8 Sprite slot if it could spawn, UCHAR_MAX otherwise
 */
u8 SpawnSprite(u8 x, u8 y, u8 type, u8 part);

/**
 * @brief Updates the sprites
 * 
 */
void UpdateSprites(void);

#endif /* SPRITE_H */
