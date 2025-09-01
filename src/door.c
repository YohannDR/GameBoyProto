#include "door.h"

#include "gb/display.h"
#include "gb/io.h"

#include "bg.h"
#include "io.h"
#include "fading.h"
#include "macros.h"
#include "scroll.h"
#include "callbacks.h"
#include "room.h"
#include "game_state.h"
#include "player.h"

#include "data/doors.h"

#define ROOM_TRANSITION_SPEED (HALF_BLOCK_SIZE)
#define ROOM_TRANSITION_FADE_SPEED (CONVERT_SECONDS(.1f))

struct DoorTransition {
    u8 direction;
    u8 timer;
    u8 tilesetToLoad;
    const struct Door* targetDoor;
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
    if (door->x - gCamera.x < SCREEN_SIZE_X_SUB_PIXEL / 2)
        gDoorTransition.direction = TILEMAP_UPDATE_LEFT;
    else
        gDoorTransition.direction = TILEMAP_UPDATE_RIGHT;
}

void DoorUpdate(void)
{
    u8 i;
    const struct Door* door;

    door = gDoors;
    for (i = 0; i < gCurrentNumberOfDoors; i++, door++)
    {
        if (gPlayerData.x + PLAYER_WIDTH < door->x)
            continue;

        if (gPlayerData.y < door->y)
            continue;

        if (gPlayerData.x > door->x + door->width * BLOCK_SIZE)
            continue;

        if (gPlayerData.y - PLAYER_HEIGHT > door->y + door->height * BLOCK_SIZE)
            continue;

        gDoorTransition.tilesetToLoad = door->tileset;
        gDoorTransition.targetDoor = &sDoors[door->targetDoor];

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
    gPlayerData.x = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor->x + 1);
    gPlayerData.y = BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor->y + 2 + gDoorTransition.targetDoor->height);

    gPlayerMovement.xVelocity = 0;
    gPlayerMovement.yVelocity = 0;
    gPlayerMovement.grounded = TRUE;

    if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
    {
        gPlayerData.x += BLOCK_TO_SUB_PIXEL(gDoorTransition.targetDoor->width) + PIXEL_SIZE;
    }
    else if (gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
    {
        gPlayerData.x -= PLAYER_WIDTH + PIXEL_SIZE;
    }

    SetupCameraForTransition();
}

static void TransitionFadeOut(void)
{
    if (gColorFading.target != FADING_TARGET_NONE)
    {
        // We wait for the fading to finish
        return;
    }

    gGameMode.sub = TRANSITION_STAGE_TRANSITION;
    gGameMode.timer = 0;

    LoadRoom(gDoorTransition.targetDoor->ownerRoom, FALSE);

    PrepareTransition();
}

void TransitionProcess(void)
{
    gGameMode.timer++;

    SetupTilemapUpdateX(TILEMAP_UPDATE_RIGHT);

    gCamera.right++;
    gBackgroundInfo.blockX++;

    // Overdraw a column
    if (gGameMode.timer != SCREEN_SIZE_X_BLOCK + 2)
        return;

    // Offset back to proper position (screen size + 1)
    gCamera.right--;

    // Fade back to the original palette
    FadingStart(FADING_TARGET_BACKGROUND, gBackgroundPalette, ROOM_TRANSITION_FADE_SPEED);
    FadingStart(FADING_TARGET_OBJ0, gObj0Palette, ROOM_TRANSITION_FADE_SPEED);
    FadingStart(FADING_TARGET_OBJ1, gObj1Palette, ROOM_TRANSITION_FADE_SPEED);

    gGameMode.sub = TRANSITION_STAGE_FADING_IN;

    gBackgroundX = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    gBackgroundY = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);
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
    else if (gGameMode.sub == TRANSITION_STAGE_TRANSITION)
    {
        TransitionProcess();
    }
    else if (gGameMode.sub == TRANSITION_STAGE_FADING_IN)
    {
        TransitionFadeIn();
    }
}
