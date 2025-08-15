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
    // X position, in sub pixels
    u16 x;
    // Y position, in sub pixels
    u16 y;
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
    // Where the graphics for that sprite are allocated in VRAM (id of the first tile)
    u8 gfxSlot;

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
 * @brief Information about the multi frame sprite graphics loading process
 * 
 */
struct SpriteLoaderInfo {
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

enum SpriteLoaderInfoState {
    SPRITE_LOADER_OFF,
    SPRITE_LOADER_ON,
    SPRITE_LOADER_LAST_UPDATE,
};

/**
 * @brief The different types of sprites
 * 
 */
enum SpriteType {
    STYPE_NONE,
    STYPE_PORTAL,
    STYPE_FIRE,
    STYPE_SHIP,
    STYPE_WATER_DROP,
    STYPE_MOVING_PLATFORM,

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
extern struct SpriteLoaderInfo gSpriteLoaderInfo;

/**
 * @brief Spawns a new sprite
 * 
 * @param x X position
 * @param y Y position
 * @param type Sprite type
 * @param part Sprite part, used to determine the specific behavior of a sprite
 * @param gfxSlot Graphics slot
 * @return u8 Sprite slot if it could spawn, UCHAR_MAX otherwise
 */
u8 SpawnSprite(u16 x, u16 y, u8 type, u8 part, u8 gfxSlot);

/**
 * @brief Finds the specified sprite with a certain part
 * 
 * @param type Sprite ItypeD
 * @param part Sprite part
 * @return u8 Sprite slot if it was found, UCHAR_MAX otherwise
 */
u8 FindSprite(u8 type, u8 part);

/**
 * @brief Prepares the process of loading sprites graphics
 * 
 */
void PrepareSpriteGraphicsLoading(void);

/**
 * @brief Starts the processus of loading sprites graphics during v-blank
 * 
 */
void StartSpriteGraphicsLoading(void);

/**
 * @brief Updates the loading of the sprites graphics
 * 
 */
void UpdateSpriteGraphicsLoading(void);

/**
 * @brief Queues a sprite's graphics (if necessary)
 * 
 * @param spriteId Sprite id
 * @return u8 Graphics tile index
 */
u8 QueueSpriteGraphics(u8 spriteId);

/**
 * @brief Updates the sprites
 * 
 */
void UpdateSprites(void);

#endif /* SPRITE_H */
