#include "sprite.h"

#include "gb/oam.h"
#include "gb/io.h"
#include "gb/display.h"

#include "macros.h"
#include "bg.h"

#include "data/sprite_data.h"

extern const struct AnimData sSpriteDefaultAnim[];

struct Sprite gSpriteData[20];

// This might seem sub-optimal, as this requires a lot of memcpy per frame (2 * sprite amount)
// But this is a relatively low overhead compared to passing the current sprite as a pointer as a parameter
struct Sprite gCurrentSprite;

// Similar to gCurrentSprite, but used by other functions, this is necessary to avoid cloberring gCurrentSprite
static struct Sprite gSpriteBuffer;

/**
 * @brief Info about allocated sprite graphics
 * 
 */
struct SpriteGraphics {
    u8 spriteId;
    u8 gfxSlot;
};

/**
 * @brief Currently allocated sprite graphics
 * 
 */
static struct SpriteGraphics gSpriteGraphics[10];
static u8 gSpriteGraphicsFreeIndex;

#define SPRITE_DRAW_FLAGS_CHECK (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN | SPRITE_STATUS_NOT_DRAWN)
#define SPRITE_DRAW_FLAGS_COND (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN)

void SpriteDraw(void)
{
    u8* oam;
    const u8* oamData;
    u8 partCount;
    u8 i;
    u8 x;
    u8 y;
    u8 properties;

    // Get the target slot in the oam buffer
    oam = OAM_BUFFER_SLOT(gNextOamSlot);

    // Get the sprite's current oam data
#ifdef HACKY_OPTIMIZATIONS
    oamData = HACKY_ARRAY_INDEXING(gCurrentSprite.animPointer, gCurrentSprite.currentAnimFrame, struct AnimData)->oamPointer;
#else
    oamData = gCurrentSprite.animPointer[gCurrentSprite.currentAnimFrame].oamPointer;
#endif

    // Get the part count, it's always the first element
    partCount = *oamData++;

    // Get the sprite's attributes
    x = SUB_PIXEL_TO_PIXEL(gCurrentSprite.x) - gBackgroundInfo.x;
    y = SUB_PIXEL_TO_PIXEL(gCurrentSprite.y) - gBackgroundInfo.y;

    properties = (gCurrentSprite.properties & SPRITE_PROPERTY_GFX) << 4;

    for (i = 0; i < partCount; i++)
    {
        // Copy all of the raw oam and add the attributes

        // Y position
        *oam++ = (*oamData++) + y;
        // X position
        *oam++ = (*oamData++) + x;
        // Tile index
        *oam++ = (*oamData++) + gCurrentSprite.gfxSlot;
        // Attribute flags
        *oam++ = (*oamData++) ^ properties;
    }

    // Update the next free oam slot
    gNextOamSlot += partCount;
}

static void SpriteUpdateOnScreenFlag(void)
{
    // TODO implement
    gCurrentSprite.status |= SPRITE_STATUS_ON_SCREEN;
}

static void SpriteUpdateAnimation(void)
{
    const struct AnimData* anim;

#ifdef HACKY_OPTIMIZATIONS
    anim = HACKY_ARRAY_INDEXING(gCurrentSprite.animPointer, gCurrentSprite.currentAnimFrame, struct AnimData);
#else
    anim = &gCurrentSprite.animPointer[gCurrentSprite.currentAnimFrame];
#endif
    gCurrentSprite.animTimer++;

    if (gCurrentSprite.animTimer >= anim->duration)
    {
        gCurrentSprite.animTimer = 0;
        gCurrentSprite.currentAnimFrame++;
        anim++;

        if (anim->duration == 0)
        {
            gCurrentSprite.currentAnimFrame = 0;
            gCurrentSprite.status |= SPRITE_STATUS_ANIM_ENDED;
        }
    }
}

u8 SpawnSprite(u16 x, u16 y, u8 type, u8 part, u8 gfxSlot)
{
    u8 i;
    struct Sprite* sprite;

    for (i = 0; i < ARRAY_SIZE(gSpriteData); i++)
    {
        sprite = &gSpriteData[i];

        if (sprite->status & SPRITE_STATUS_EXISTS)
            continue;

        gSpriteBuffer = *sprite;

        // Set parameters and default values
        gSpriteBuffer.status = SPRITE_STATUS_EXISTS;
        gSpriteBuffer.x = x;
        gSpriteBuffer.y = y;
        gSpriteBuffer.type = type;
        gSpriteBuffer.part = part;
        gSpriteBuffer.ramSlot = i;
        gSpriteBuffer.gfxSlot = gfxSlot;
        
        // And zero-out the rest
        gSpriteBuffer.pose = 0;
        gSpriteBuffer.currentAnimFrame = 0;
        gSpriteBuffer.animTimer = 0;
        gSpriteBuffer.animPointer = sSpriteDefaultAnim;
        gSpriteBuffer.work1 = 0;
        gSpriteBuffer.work2 = 0;
        gSpriteBuffer.work3 = 0;
        gSpriteBuffer.work4 = 0;

        *sprite = gSpriteBuffer;

        return i;
    }

    return UCHAR_MAX;
}

u8 LoadSpriteGraphics(u8 spriteId)
{
    u8 i;
    u8* dst;
    const u8* src;
    u8 size;
    u8 gfxSlot;

    for (i = 0; i < ARRAY_SIZE(gSpriteGraphics); i++)
    {
        if (gSpriteGraphics[i].spriteId == spriteId)
        {
            // Graphics for this sprite have already been loaded
            return gSpriteGraphics[i].gfxSlot;
        }

        if (gSpriteGraphics[i].spriteId != STYPE_NONE)
        {
            // This slot is already used
            continue;
        }

        gfxSlot = gSpriteGraphicsFreeIndex;

        // Found free slot
        gSpriteGraphics[i].spriteId = spriteId;
        gSpriteGraphics[i].gfxSlot = gSpriteGraphicsFreeIndex;

        src = sSpriteGraphicsPointers[spriteId];
        size = *src++;

        dst = (u8*)(VRAM_BASE + gSpriteGraphicsFreeIndex * 16);
        gSpriteGraphicsFreeIndex += size;

        // We can safely re-use i here, we don't use it again and we return right after
        for (i = 0; i < size; i++)
        {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
        }

        return gfxSlot;
    }

    return 0;
}

void UpdateSprites(void)
{
    u8 i;
    struct Sprite* sprite;

    for (i = 0; i < ARRAY_SIZE(gSpriteData); i++)
    {
        sprite = &gSpriteData[i];

        if (!(sprite->status & SPRITE_STATUS_EXISTS))
            continue;

        gCurrentSprite = *sprite;

        // Clear this flag, it's only active during the exact frame the animation ended
        gCurrentSprite.status &= ~SPRITE_STATUS_ANIM_ENDED;

        SpriteUpdateOnScreenFlag();
        SpriteUpdateAnimation();

        // Call AI
        sSpriteAiPointers[gCurrentSprite.type]();

        if ((gCurrentSprite.status & SPRITE_DRAW_FLAGS_CHECK) == SPRITE_DRAW_FLAGS_COND)
            SpriteDraw();

        *sprite = gCurrentSprite;
    }
}

static const u8 sSpriteDefaultAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(0), 0, 0,
};

const struct AnimData sSpriteDefaultAnim[] = {
    [0] = {
        .oamPointer = sSpriteDefaultAnim_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};
