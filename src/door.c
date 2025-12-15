#include "door.h"

#include "gb/display.h"
#include "gb/io.h"

#include "bg.h"
#include "io.h"
#include "input.h"
#include "fading.h"
#include "macros.h"
#include "scroll.h"
#include "callbacks.h"
#include "room.h"
#include "game_state.h"
#include "gfx_loader.h"
#include "player.h"

#include "data/doors.h"
#include "data/tilesets.h"

#define ROOM_TRANSITION_SPEED (HALF_BLOCK_SIZE)
#define ROOM_TRANSITION_FADE_SPEED (CONVERT_SECONDS(.1f))

struct DoorTransition {
    u8 direction;
    u8 timer;
    u8 tilesetToLoad;
    struct Door targetDoor;
};

enum TransitionStage {
    TRANSITION_STAGE_NONE,
    TRANSITION_STAGE_FADING_OUT,
    TRANSITION_STAGE_LOADING_TILEMAP,
    TRANSITION_STAGE_LOADING_SPRITES,
    TRANSITION_STAGE_LOADING_TILESET,
    TRANSITION_STAGE_FADING_IN,
};

struct Door gDoors[4];
static u8 gCurrentNumberOfDoors;

static struct DoorTransition gDoorTransition;

void DoorReset(void)
{
    gCurrentNumberOfDoors = 0;
}

void DoorLoad(const struct Door* door)
{
    gDoors[gCurrentNumberOfDoors] = *door;
    gDoors[gCurrentNumberOfDoors].x = BLOCK_TO_SUB_PIXEL(gDoors[gCurrentNumberOfDoors].x + 1);
    gDoors[gCurrentNumberOfDoors].y = BLOCK_TO_SUB_PIXEL(gDoors[gCurrentNumberOfDoors].y + 2);

    gCurrentNumberOfDoors++;
}

static void DetermineTransitionDirection(const struct Door* door)
{
    // We use the "shape" of the door to determine its orientation
    // Then we compute the screen space position of the door to see on which side it is
    if (door->height > door->width)
    {
        if (door->x - gCamera.x < SCREEN_SIZE_X_SUB_PIXEL / 2)
            gDoorTransition.direction = TILEMAP_UPDATE_LEFT;
        else
            gDoorTransition.direction = TILEMAP_UPDATE_RIGHT;
    }
    else
    {
        if (door->y - gCamera.y < SCREEN_SIZE_Y_SUB_PIXEL / 2)
            gDoorTransition.direction = TILEMAP_UPDATE_TOP;
        else
            gDoorTransition.direction = TILEMAP_UPDATE_BOTTOM;
    }
}

void DoorUpdate(void)
{
    u8 i;
    const struct Door* door;

    door = gDoors;
    for (i = 0; i < gCurrentNumberOfDoors; i++, door++)
    {
        if (gPlayerData.x + PLAYER_WIDTH / 3 * 2 < door->x)
            continue;

        if (gPlayerData.y < door->y)
            continue;

        if (gPlayerData.x + PLAYER_WIDTH / 3 * 1 > door->x + BLOCK_TO_SUB_PIXEL(door->width))
            continue;

        if (gPlayerData.y - PLAYER_HEIGHT > door->y + BLOCK_TO_SUB_PIXEL(door->height))
            continue;

        // That's kind of hacky, but it kind of works I guess
        // since every "background" door is an area transition of sorts they also always load tilesets
        if (door->tileset != UCHAR_MAX)
        {
            // Only go through the door if up is pressed
            if (!(gChangedInput & KEY_UP))
                continue;
        }

        gDoorTransition.tilesetToLoad = door->tileset;

        // Fully copy it because it's in another bank so it's a pain to use a pointer
        gDoorTransition.targetDoor = sDoors[door->targetDoor];

        gGameMode.main = GM_TRANSITION;
        gGameMode.sub = TRANSITION_STAGE_FADING_OUT;

        DetermineTransitionDirection(door);
        FadingStart(FADING_TARGET_BACKGROUND | FADING_TARGET_OBJ0 | FADING_TARGET_OBJ1, PALETTE_BLACK, ROOM_TRANSITION_FADE_SPEED);
    }
}

static void SetupCameraForTransition(void)
{
    u16 targetX;
    u16 targetY;

    targetX = GetCameraTargetX();
    targetY = GetCameraTargetY();

    SetCameraPosition(targetX, targetY);

    gBackgroundInfo.blockX -= SCREEN_SIZE_X_BLOCK;
    gCamera.left--;
    gCamera.right -= SCREEN_SIZE_X_BLOCK + 1;
}

static void PrepareTransition(void)
{
    u16 width;

    // Set at origin of door
    gPlayerData.x = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.x + 1);
    gPlayerData.y = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.y + 2);

    gPlayerData.carryingObject = FALSE;

    gPlayerMovement.xVelocity = 0;
    gPlayerMovement.yVelocity = 0;
    gPlayerMovement.grounded = TRUE;

    if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT || gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
    {
        // Move the player at the bottom of the door
        gPlayerData.y += BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.height);

        if (gDoorTransition.tilesetToLoad != UCHAR_MAX)
        {
            width = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.width);
            gPlayerData.x += width / 2 - PLAYER_WIDTH / 2;
        }
        else
        {
            // Put the player on the side of the door
            if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
                gPlayerData.x += BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.width) + PIXEL_SIZE;
            else
                gPlayerData.x -= PLAYER_WIDTH + PIXEL_SIZE;
        }
    }
    else
    {
        width = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.width);
        gPlayerData.x += BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.width) / 2 - PLAYER_WIDTH / 2;

        if (gDoorTransition.direction == TILEMAP_UPDATE_TOP)
        {
            // Force a jump, otherwise the player will immediatly fall into the transition
            gPlayerMovement.yVelocity = gPlayerPhysics.jumpingVelocity;
            gPlayerMovement.grounded = FALSE;

            gPlayerData.y -= BLOCK_SIZE + PIXEL_SIZE;
        }
        else
        {
            gPlayerData.y += BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor.width) + PLAYER_HEIGHT + PIXEL_SIZE;
        }
    }

    SetupCameraForTransition();
}

static void StartTilesetLoading()
{
    const u8* src;

    StartSpriteGraphicsLoading();

    if (gGraphicsLoaderInfo.state != GRAPHICS_LOADER_OFF)
    {
        gGameMode.sub = TRANSITION_STAGE_LOADING_SPRITES;
        return;
    }

    if (gDoorTransition.tilesetToLoad == UCHAR_MAX)
    {
        // Fade back to the original palette
        FadingStart(FADING_TARGET_BACKGROUND, gBackgroundPalette, ROOM_TRANSITION_FADE_SPEED);
        FadingStart(FADING_TARGET_OBJ0, gObj0Palette, ROOM_TRANSITION_FADE_SPEED);
        FadingStart(FADING_TARGET_OBJ1, gObj1Palette, ROOM_TRANSITION_FADE_SPEED);

        gGameMode.sub = TRANSITION_STAGE_FADING_IN;
        return;
    }

    src = sTilesets[gDoorTransition.tilesetToLoad];

    gGraphicsLoaderInfo.nbrTilesToLoad = *src++;
    gGraphicsLoaderInfo.gfxAddr = src;

    gGraphicsLoaderInfo.state = GRAPHICS_LOADER_ON | GRAPHICS_LOADER_TILESET;
    gGraphicsLoaderInfo.vramAddr = (u8*)(VRAM_BASE + 0x1000 - ARRAY_SIZE(gGraphicsLoaderBuffer));
    gGraphicsLoaderInfo.nbrTilesLoaded = 0;
    gGraphicsLoaderInfo.nbrBytesBuffered = 0;

    gGameMode.sub = TRANSITION_STAGE_LOADING_TILESET;
}

static void TransitionLoadTilemap(void)
{
    u8 i;

    if (gGraphicsLoaderInfo.state == (GRAPHICS_LOADER_LAST_UPDATE | GRAPHICS_LOADER_TILEMAP))
    {
        gGraphicsLoaderInfo.state = GRAPHICS_LOADER_OFF;
        StartTilesetLoading();
        return;
    }

    gGraphicsLoaderInfo.vramAddr += ARRAY_SIZE(gGraphicsLoaderBuffer);

    for (i = 0; i < ARRAY_SIZE(gGraphicsLoaderBuffer) + 1; i++)
    {
        gGraphicsLoaderBuffer[i] = gGraphicsLoaderInfo.gfxAddr[i];

        if (i != 0 && i % 16 == 0)
        {
            gGraphicsLoaderInfo.nbrTilesLoaded++;

            // Check if we've fully loaded these graphics
            if (gGraphicsLoaderInfo.nbrTilesLoaded == gGraphicsLoaderInfo.nbrTilesToLoad)
            {
                // There's no more graphics to load so we're done, but we can't turn off the loader yet,
                // because otherwise what we just buffered will never be sent to VRAM, so we keep it in a "half" active state for a single frame
                gGraphicsLoaderInfo.state = GRAPHICS_LOADER_LAST_UPDATE | GRAPHICS_LOADER_TILEMAP;
                // We might not have filled the buffer with valid data, so we only mark to load what we actually buffered
                gGraphicsLoaderInfo.nbrBytesBuffered = i;
                return;
            }

            // Since the loop goes to buffer size + 1, we have to check if we should exit early to not r/w out of bounds
            if (i == ARRAY_SIZE(gGraphicsLoaderBuffer))
                break;
        }
    }

    gGraphicsLoaderInfo.gfxAddr += i;
    gGraphicsLoaderInfo.nbrBytesBuffered = ARRAY_SIZE(gGraphicsLoaderBuffer);
}


static void TransitionFadeOut(void)
{
    if (gColorFading.target != FADING_TARGET_NONE)
    {
        // We wait for the fading to finish
        return;
    }

    gGameMode.sub = TRANSITION_STAGE_LOADING_TILEMAP;
    gGameMode.timer = 0;

    LoadRoom(gDoorTransition.targetDoor.ownerRoom);

    PrepareTransition();
}

static void TransitionLoadTileset(void)
{
    u8 i;

    if (gGraphicsLoaderInfo.state == (GRAPHICS_LOADER_LAST_UPDATE | GRAPHICS_LOADER_TILESET))
    {
        // At this point, we're on the frame after the last update, so the last graphics have been sent to VRAM properly
        // thus we can safely turn off the loader
        gGraphicsLoaderInfo.state = GRAPHICS_LOADER_OFF;

        gGameMode.sub = TRANSITION_STAGE_FADING_IN;

        // Fade back to the original palette
        FadingStart(FADING_TARGET_BACKGROUND, gBackgroundPalette, ROOM_TRANSITION_FADE_SPEED);
        FadingStart(FADING_TARGET_OBJ0, gObj0Palette, ROOM_TRANSITION_FADE_SPEED);
        FadingStart(FADING_TARGET_OBJ1, gObj1Palette, ROOM_TRANSITION_FADE_SPEED);
        return;
    }

    // Advance VRAM pointer, the buffer starts offset'd as explained above, so this is "behind" when we enter the function
    gGraphicsLoaderInfo.vramAddr += ARRAY_SIZE(gGraphicsLoaderBuffer);

    if (gGraphicsLoaderInfo.vramAddr == (u8*)(VRAM_BASE + 0x1800))
        gGraphicsLoaderInfo.vramAddr = (u8*)(VRAM_BASE + 0x800);

    // Loop goes to buffer size + 1 because it's necessary to properly perform the tile buffering check
    // if we stopped at the size, we would "miss" the last tile
    // For example if the buffer could only contain a single tile, it would have size 16, and the loop would go from 0 to 15
    // but then the i % 16 == 0 check to see if we've buffered a tile would never be true,
    // since it would require i to reach 16 which isn't in the loop range
    for (i = 0; i < ARRAY_SIZE(gGraphicsLoaderBuffer) + 1; i++)
    {
        // Check if we've buffered an entire tile
        if (i != 0 && i % 16 == 0)
        {
            gGraphicsLoaderInfo.nbrTilesLoaded++;

            // Check if we've fully loaded these graphics
            if (gGraphicsLoaderInfo.nbrTilesLoaded == gGraphicsLoaderInfo.nbrTilesToLoad)
            {
                // There's no more graphics to load so we're done, but we can't turn off the loader yet,
                // because otherwise what we just buffered will never be sent to VRAM, so we keep it in a "half" active state for a single frame
                gGraphicsLoaderInfo.state = GRAPHICS_LOADER_LAST_UPDATE | GRAPHICS_LOADER_TILESET;
                // We might not have filled the buffer with valid data, so we only mark to load what we actually buffered
                gGraphicsLoaderInfo.nbrBytesBuffered = i;
                return;
            }

            // Since the loop goes to buffer size + 1, we have to check if we should exit early to not r/w out of bounds
            if (i == ARRAY_SIZE(gGraphicsLoaderBuffer))
                break;
        }

        // Write to the buffer
        gGraphicsLoaderBuffer[i] = gGraphicsLoaderInfo.gfxAddr[i];
    }

    // Advance the source graphics by what was loaded
    gGraphicsLoaderInfo.gfxAddr += i;
    // If we reached this part, it's garanteed we fully filled the buffer
    gGraphicsLoaderInfo.nbrBytesBuffered = ARRAY_SIZE(gGraphicsLoaderBuffer);
}

static void TransitionFadeIn(void)
{
    if (gColorFading.target != FADING_TARGET_NONE)
    {
        // We wait for the fading to finish
        return;
    }

    // Return in game
    gGameMode.main = GM_IN_GAME;
}

void TransitionUpdate(void)
{
    if (gGameMode.sub == TRANSITION_STAGE_FADING_OUT)
    {
        TransitionFadeOut();
    }
    else if (gGameMode.sub == TRANSITION_STAGE_LOADING_TILEMAP)
    {
        TransitionLoadTilemap();
    }
    else if (gGameMode.sub == TRANSITION_STAGE_LOADING_SPRITES)
    {
        if (gGraphicsLoaderInfo.state == GRAPHICS_LOADER_OFF)
        {
            TransitionLoadTileset();
        }
    }
    else if (gGameMode.sub == TRANSITION_STAGE_LOADING_TILESET)
    {
        TransitionLoadTileset();
    }
    else if (gGameMode.sub == TRANSITION_STAGE_FADING_IN)
    {
        TransitionFadeIn();
    }
}
