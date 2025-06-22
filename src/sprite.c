#include "sprite.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "macros.h"
#include "bg.h"

#include "data/sprite_data.h"

struct Sprite gSpriteData[20];

#define SPRITE_DRAW_FLAGS_CHECK (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN | SPRITE_STATUS_NOT_DRAWN)
#define SPRITE_DRAW_FLAGS_COND (SPRITE_STATUS_EXISTS)

static void SpriteDraw(const struct Sprite* sprite)
{
    struct Oam* oam = &gOamBuffer[gNextOamSlot];

    oam->y = sprite->y;
    oam->x = sprite->x;
    oam->tile = 3;

    gNextOamSlot++;
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

        // And zero-out the rest
        sprite->pose = 0;
        sprite->currentAnimFrame = 0;
        sprite->animTimer = 0;
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

        SpriteUpdateOnScreenFlag(sprite);

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
