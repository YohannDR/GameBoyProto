#include "sprite.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "macros.h"
#include "bg.h"

#include "data/sprite_data.h"

struct Sprite gSpriteData[20];

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

void SpriteDraw(const struct Sprite* sprite)
{
    struct Oam* oam;
    u8* rawOam;
    const u8* oamData;
    u8 partCount;
    u8 i;
    u8 x;
    u8 y;
    u8 properties;

    // Get the target slot in the oam buffer
    oam = &gOamBuffer[gNextOamSlot];
    // Get a raw pointer to that slot
    rawOam = (u8*)oam;
    // Get the sprite's current oam data
    oamData = sprite->animPointer[sprite->currentAnimFrame].oamPointer;
    // Get the part count, it's always the first element
    partCount = *oamData++;

    // Get the sprite's attributes
    x = sprite->x;
    y = sprite->y;
    properties = (sprite->properties & SPRITE_PROPERTY_GFX) << 4;

    for (i = 0; i < partCount; i++)
    {
        // First, copy all of the raw oam
        *rawOam++ = *oamData++;
        *rawOam++ = *oamData++;
        *rawOam++ = *oamData++;
        *rawOam++ = *oamData++;

        // Then apply the attributes
        oam->y += y;
        oam->x += x;
        oam->attributes |= properties;

        oam++;
    }

    // Update the next free oam slot
    gNextOamSlot += partCount;
}

static void SpriteUpdateOnScreenFlag(struct Sprite* sprite)
{
    if (sprite->x > gBackgroundX && sprite->x < gBackgroundX + SCREEN_SIZE_X &&
        sprite->y > gBackgroundY && sprite->y < gBackgroundY + SCREEN_SIZE_Y)
    {
        sprite->status |= SPRITE_STATUS_ON_SCREEN;
    }
    else
    {
        sprite->status &= ~SPRITE_STATUS_ON_SCREEN;
    }
}

static void SpriteUpdateAnimation(struct Sprite* sprite)
{
    const struct AnimData* anim;

    anim = &sprite->animPointer[sprite->currentAnimFrame];
    sprite->animTimer++;

    if (sprite->animTimer >= anim->duration)
    {
        sprite->animTimer = 0;
        sprite->currentAnimFrame++;
        anim++;

        if (anim->duration == 0)
        {
            sprite->currentAnimFrame = 0;
            sprite->status |= SPRITE_STATUS_ANIM_ENDED;
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

        // Set parameters and default values
        sprite->status = SPRITE_STATUS_EXISTS;
        sprite->x = x;
        sprite->y = y;
        sprite->type = type;
        sprite->ramSlot = i;
        
        // And zero-out the rest
        sprite->pose = 0;
        sprite->currentAnimFrame = 0;
        sprite->animTimer = 0;
        sprite->animPointer = sSpriteDefaultAnim;
        sprite->work1 = 0;
        sprite->work2 = 0;
        sprite->work3 = 0;
        sprite->work4 = 0;

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

        sprite->status &= ~SPRITE_STATUS_ANIM_ENDED;

        SpriteUpdateOnScreenFlag(sprite);
        SpriteUpdateAnimation(sprite);

        // Call AI
        sSpriteAiPointers[sprite->type](sprite);
    }
}

void DrawSprites(void)
{
    u8 i;
    const struct Sprite* sprite;

    for (i = 0; i < ARRAY_SIZE(gSpriteData); i++)
    {
        sprite = &gSpriteData[i];

        if (sprite->status & SPRITE_STATUS_EXISTS)
        // if ((sprite->status & SPRITE_DRAW_FLAGS_CHECK) == SPRITE_DRAW_FLAGS_COND)
            SpriteDraw(sprite);
    }
}
