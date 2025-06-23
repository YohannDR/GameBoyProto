#include "sprite.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "macros.h"
#include "bg.h"

#include "data/sprite_data.h"

struct Sprite gSpriteData[20];

// This might seem sub-optimal, as this requires a lot of memcpy per frame (2 * sprite amount)
// But this is a relatively low overhead compared to passing the current sprite as a pointer as a parameter
struct Sprite gCurrentSprite;

// Similar to gCurrentSprite, but used by other functions, this is necessary to avoid cloberring gCurrentSprite
static struct Sprite gSpriteBuffer;

#define SPRITE_DRAW_FLAGS_CHECK (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN | SPRITE_STATUS_NOT_DRAWN)
#define SPRITE_DRAW_FLAGS_COND (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN)

static const u8 sSpriteDataDefaultAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 0, 0
};

static const struct AnimData sSpriteDefaultAnim[] = {
    [0] = {
        .oamPointer = sSpriteDataDefaultAnim_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

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
    // The cast to u8 prevents integer promotion, which generates unnecessary code
    oam = &gOamBuffer[(u8)(gNextOamSlot * 4)];

    // Get the sprite's current oam data
#ifdef HACKY_OPTIMIZATIONS
    // This is kind of bullshit, but it's the same reason as above
    // Since the compiler can't know if caf * 3 will overflow above 255, it has to promote it to a u16
    // which makes the array indexing slower
    // So the optimization boils down to doing the array indexing ourselves by treating it as a u8* and performing the
    // multiplication ourselves, which keeps the array index as an u8 during the multiplication
    // This does have a side effect of breaking indexing if caf > 85, but that reasonably won't happen, probably
    oamData = *(u8**)(((u8*)gCurrentSprite.animPointer) + (u8)(gCurrentSprite.currentAnimFrame * sizeof(struct AnimData)));
#else
    oamData = gCurrentSprite.animPointer[gCurrentSprite.currentAnimFrame].oamPointer;
#endif

    // Get the part count, it's always the first element
    partCount = *oamData++;

    // Get the sprite's attributes
    x = gCurrentSprite.x;
    y = gCurrentSprite.y;
    properties = (gCurrentSprite.properties & SPRITE_PROPERTY_GFX) << 4;

    for (i = 0; i < partCount; i++)
    {
        // Copy all of the raw oam and add the attributes

        // Y position
        *oam++ = (*oamData++) + y;
        // X position
        *oam++ = (*oamData++) + x;
        // Tile index
        *oam++ = (*oamData++);
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

    anim = &gCurrentSprite.animPointer[gCurrentSprite.currentAnimFrame];
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


u8 SpawnSprite(u8 x, u8 y, u8 type)
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
        gSpriteBuffer.ramSlot = i;
        
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
