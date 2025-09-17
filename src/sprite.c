#include "sprite.h"

#include "gb/oam.h"
#include "gb/io.h"
#include "gb/display.h"

#include "bank.h"
#include "bg.h"
#include "gfx_loader.h"
#include "macros.h"

#include "data/sprite_data.h"

#define PLAYER_GRAPHICS_RESERVED_AREA 16

extern const struct AnimData sSpriteDefaultAnim[];

struct Sprite gSpriteData[20];
u8 gEnemiesLeftToProcess;
u8 gOamPartsLeftToProcess;
u8 gSpriteDrawAttributes;

u8 gSpriteScreenX;
u8 gSpriteScreenY;

u8 gMaxAmountOfExistingSprites;

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

/**
 * @brief The index of the next free sprite graphics tile
 * 
 */
static u8 gSpriteGraphicsFreeIndex;

/**
 * @brief The sprite graphics that are currently queued to be loaded
 * 
 */
const u8* gQueuedGraphics[10];

/**
 * @brief Index into the sprite graphics queue
 * 
 */
static u8 gQueuedGraphicsIndex;

#define SPRITE_DRAW_FLAGS_CHECK (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN | SPRITE_STATUS_NOT_DRAWN)
#define SPRITE_DRAW_FLAGS_COND (SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN)

// I'm only commenting these functions out for legacy reasons
/*
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
    x = SUB_PIXEL_TO_PIXEL(gCurrentSprite.x) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    y = SUB_PIXEL_TO_PIXEL(gCurrentSprite.y) - SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);

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

void SpriteUpdateOnScreenFlag(void)
{
    u16 left;
    u16 right;
    u16 top;
    u16 bottom;
    u16 bgX;
    u16 bgY;

    bgX = gBackgroundInfo.x + BLOCK_SIZE;

    right = gCurrentSprite.x + PIXEL_TO_SUB_PIXEL(gCurrentSprite.drawDistanceRight);
    if (bgX >= right)
    {
        gCurrentSprite.status &= ~SPRITE_STATUS_ON_SCREEN;
        return;
    }

    left = gCurrentSprite.x + PIXEL_TO_SUB_PIXEL(gCurrentSprite.drawDistanceLeft);
    if (bgX + SCREEN_SIZE_X_SUB_PIXEL <= left)
    {
        gCurrentSprite.status &= ~SPRITE_STATUS_ON_SCREEN;
        return;
    }

    bgY = gBackgroundInfo.y + BLOCK_SIZE * 2;

    bottom = gCurrentSprite.y + PIXEL_TO_SUB_PIXEL(gCurrentSprite.drawDistanceBottom);
    if (bgY >= bottom)
    {
        gCurrentSprite.status &= ~SPRITE_STATUS_ON_SCREEN;
        return;
    }

    top = gCurrentSprite.y + PIXEL_TO_SUB_PIXEL(gCurrentSprite.drawDistanceTop);
    if (bgY + SCREEN_SIZE_Y_SUB_PIXEL <= top)
    {
        gCurrentSprite.status &= ~SPRITE_STATUS_ON_SCREEN;
        return;
    }

    gCurrentSprite.status |= SPRITE_STATUS_ON_SCREEN;
}

void SpriteUpdateAnimation(void)
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

void UpdateSprites(void)
{
    u8 i;
    struct Sprite* sprite;

    sprite = gSpriteData;
    for (i = 0; i < ARRAY_SIZE(gSpriteData); i++, sprite++)
    {
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
*/

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
        gSpriteBuffer.status = SPRITE_STATUS_EXISTS | SPRITE_STATUS_ON_SCREEN;
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
        gSpriteBuffer.properties = 0;
        gSpriteBuffer.drawDistanceLeft = 0;
        gSpriteBuffer.drawDistanceRight = 0;
        gSpriteBuffer.drawDistanceTop = 0;
        gSpriteBuffer.drawDistanceBottom = 0;
        gSpriteBuffer.work1 = 0;
        gSpriteBuffer.work2 = 0;
        gSpriteBuffer.work3 = 0;
        gSpriteBuffer.work4 = 0;

        *sprite = gSpriteBuffer;

        if (i >= gMaxAmountOfExistingSprites)
            gMaxAmountOfExistingSprites = i + 1;

        return i;
    }

    return UCHAR_MAX;
}

u8 FindSprite(u8 type, u8 part)
{
    u8 i;

    for (i = 0; i < ARRAY_SIZE(gSpriteData); i++)
    {
        if (!gSpriteData[i].status & SPRITE_STATUS_EXISTS)
            continue;

        if (gSpriteData[i].type == type)
        {
            if (part == UCHAR_MAX)
                return i;

            if (gSpriteData[i].part == part)
                return i;
        }
    }

    return UCHAR_MAX;
}

void PrepareSpriteGraphicsLoading(void)
{
    u8 i;

    gSpriteGraphicsFreeIndex = PLAYER_GRAPHICS_RESERVED_AREA;
    gQueuedGraphicsIndex = 0;

    for (i = 0; i < ARRAY_SIZE(gQueuedGraphics); i++)
        gQueuedGraphics[i] = NULL;

    for (i = 0; i < ARRAY_SIZE(gSpriteGraphics); i++)
        gSpriteGraphics[i].spriteId = STYPE_NONE;
}

void StartSpriteGraphicsLoading(void)
{
    const u8* src;

    SwitchBank(BANK_SPRITE);

    // We reset the counter, we'll be using it to count again
    gQueuedGraphicsIndex = 0;

    // Base vram adress
    // We remove the buffer size because it's always added at the beginning of the update, that way we can start at the proper '0' address
    gGraphicsLoaderInfo.vramAddr = (u8*)(VRAM_BASE + PLAYER_GRAPHICS_RESERVED_AREA * 16 - ARRAY_SIZE(gGraphicsLoaderBuffer));

    // Prepare the first queued graphics, if there's any
    src = gQueuedGraphics[0];

    if (src != NULL)
    {
        gGraphicsLoaderInfo.nbrTilesToLoad = *src++;
        gGraphicsLoaderInfo.gfxAddr = src;
        gGraphicsLoaderInfo.nbrTilesLoaded = 0;
        gGraphicsLoaderInfo.nbrBytesBuffered = 0;
        gGraphicsLoaderInfo.state = GRAPHICS_LOADER_ON;
    }

    BankGoBack();
}

void UpdateSpriteGraphicsLoading(void)
{
    u8 i;
    u8 gfxIndex;
    const u8* src;

    SwitchBank(BANK_SPRITE);

    if (gGraphicsLoaderInfo.state == GRAPHICS_LOADER_LAST_UPDATE)
    {
        // At this point, we're on the frame after the last update, so the last graphics have been sent to VRAM properly
        // thus we can safely turn off the loader
        gGraphicsLoaderInfo.state = GRAPHICS_LOADER_OFF;
        return;
    }

    // Advance VRAM pointer, the buffer starts offset'd as explained above, so this is "behind" when we enter the function
    gGraphicsLoaderInfo.vramAddr += ARRAY_SIZE(gGraphicsLoaderBuffer);

    // Loop goes to buffer size + 1 because it's necessary to properly perform the tile buffering check
    // if we stopped at the size, we would "miss" the last tile
    // For example if the buffer could only contain a single tile, it would have size 16, and the loop would go from 0 to 15
    // but then the i % 16 == 0 check to see if we've buffered a tile would never be true,
    // since it would require i to reach 16 which isn't in the loop range
    for (i = 0, gfxIndex = 0; i < ARRAY_SIZE(gGraphicsLoaderBuffer) + 1; i++, gfxIndex++)
    {
        // Check if we've buffered an entire tile
        if (i != 0 && i % 16 == 0)
        {
            gGraphicsLoaderInfo.nbrTilesLoaded++;

            // Check if we've fully loaded these graphics
            if (gGraphicsLoaderInfo.nbrTilesLoaded == gGraphicsLoaderInfo.nbrTilesToLoad)
            {
                // If yes, we can move on to next in the queue
                gQueuedGraphicsIndex++;
                src = gQueuedGraphics[gQueuedGraphicsIndex];

                if (src == NULL)
                {
                    // There's no more graphics to load so we're done, but we can't turn off the loader yet,
                    // because otherwise what we just buffered will never be sent to VRAM, so we keep it in a "half" active state for a single frame
                    gGraphicsLoaderInfo.state = GRAPHICS_LOADER_LAST_UPDATE;
                    // We might not have filled the buffer with valid data, so we only mark to load what we actually buffered
                    gGraphicsLoaderInfo.nbrBytesBuffered = i;
                    return;
                }

                // Setup
                gGraphicsLoaderInfo.nbrTilesLoaded = 0;
                gGraphicsLoaderInfo.nbrTilesToLoad = *src++;
                gGraphicsLoaderInfo.gfxAddr = src;
                // Reset the gfx index
                gfxIndex = 0;
            }

            // Since the loop goes to buffer size + 1, we have to check if we should exit early to not r/w out of bounds
            if (i == ARRAY_SIZE(gGraphicsLoaderBuffer))
                break;
        }

        // Write to the buffer
        gGraphicsLoaderBuffer[i] = gGraphicsLoaderInfo.gfxAddr[gfxIndex];
    }

    // Advance the source graphics by what was loaded
    gGraphicsLoaderInfo.gfxAddr += gfxIndex;
    // If we reached this part, it's garanteed we fully filled the buffer
    gGraphicsLoaderInfo.nbrBytesBuffered = ARRAY_SIZE(gGraphicsLoaderBuffer);
}

u8 QueueSpriteGraphics(u8 spriteId)
{
    u8 i;
    const u8* src;
    u8 size;
    u8 gfxSlot;

    // Switch to the sprite bank, required to have access to the graphics
    SwitchBank(BANK_SPRITE);

    gfxSlot = 0;

    for (i = 0; i < ARRAY_SIZE(gSpriteGraphics); i++)
    {
        if (gSpriteGraphics[i].spriteId == spriteId)
        {
            // Graphics for this sprite have already been loaded
            gfxSlot = gSpriteGraphics[i].gfxSlot;
            break;
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

        if (*src == 0)
        {
            // We can skip sprites with empty graphics
            gfxSlot = gSpriteGraphicsFreeIndex;
            break;
        }

        // Queue the graphics to be loaded
        gQueuedGraphics[gQueuedGraphicsIndex] = src;
        gQueuedGraphicsIndex++;

        size = *src++;

        gSpriteGraphicsFreeIndex += size;

        break;
    }

    // Don't forget to switch back
    BankGoBack();
    return gfxSlot;
}

void ClearSprites(void)
{
    u8 i;
    struct Sprite* sprite;

    sprite = gSpriteData;
    for (i = 0; i < gMaxAmountOfExistingSprites; i++, sprite++)
        sprite->status = 0;
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
