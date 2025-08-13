#include "door.h"

#include "bg.h"
#include "fading.h"
#include "macros.h"
#include "scroll.h"
#include "room.h"
#include "game_state.h"
#include "player.h"

#include "data/doors.h"

#define TRANSITION_SPEED 1

struct Door gDoors[4];
struct DoorTransition gDoorTransition;
static u8 gCurrentNumberOfDoors;

static void SetupSimpleDoorTransition(const struct Door* door)
{
    gDoorTransition.type = TRANSITION_TYPE_NORMAL;
    gDoorTransition.stage = TRANSITION_STAGE_NORMAL_SCROLLING;
    gDoorTransition.timer = 0;

    if (door->x - gRoomOriginX - gCamera.x < SCREEN_SIZE_X_SUB_PIXEL / 2)
        gDoorTransition.direction = TILEMAP_UPDATE_LEFT;
    else
        gDoorTransition.direction = TILEMAP_UPDATE_RIGHT;
}

static void PrepareSimpleTransition(const struct Door* door)
{
    const struct Door* dstDoor;

    gBackgroundInfo.tilemapAnchorX -= SUB_PIXEL_TO_BLOCK(gRoomOriginX);
    gBackgroundInfo.tilemapAnchorY -= SUB_PIXEL_TO_BLOCK(gRoomOriginY);

    if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
    {
        gCamera.left = UCHAR_MAX;
        gCamera.right = 0;
    }
    else if (gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
    {
        gCamera.left = gTilemap.width - 1;
        gCamera.right = gTilemap.width;
    }

    dstDoor = &sDoors[door->targetDoor];

    SetupTilemapUpdateX(gDoorTransition.direction);
}

void DoorReset(void)
{
    gCurrentNumberOfDoors = 0;
}

void DoorLoad(const struct Door* door)
{
    gDoors[gCurrentNumberOfDoors] = *door;
    gDoors[gCurrentNumberOfDoors].x = gRoomOriginX + (gDoors[gCurrentNumberOfDoors].x + 1) * BLOCK_SIZE;
    gDoors[gCurrentNumberOfDoors].y = gRoomOriginY + (gDoors[gCurrentNumberOfDoors].y + 2) * BLOCK_SIZE;

    gCurrentNumberOfDoors++;
}

void DoorUpdate(void)
{
    u8 i;
    const struct Door* door;

    if (gDoorTransition.type != TRANSITION_TYPE_NONE)
        return;

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

        gGameMode.main = GM_TRANSITION;

        if (door->tileset != gCurrentTileset && door->tileset != UCHAR_MAX)
        {
            gDoorTransition.type = TRANSITION_TYPE_LOADING;
            FadingStart(FADING_TARGET_BACKGROUND | FADING_TARGET_OBJ0 | FADING_TARGET_OBJ1, PALETTE_BLACK, 10);
        }
        else
        {
            SetupSimpleDoorTransition(door);
            TransitionToRoom(sDoors[door->targetDoor].ownerRoom);
            PrepareSimpleTransition(door);
        }
    }
}

#define ROOM_TRANSITION_SPEED HALF_BLOCK_SIZE

void TransitionUpdate(void)
{
    u8 newBlock;

    gDoorTransition.timer++;

    if (gDoorTransition.stage == TRANSITION_STAGE_NORMAL_SCROLLING)
    {
        newBlock = gDoorTransition.timer % (BLOCK_SIZE / ROOM_TRANSITION_SPEED);

        if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
        {
            gBackgroundInfo.x += ROOM_TRANSITION_SPEED;
            gPlayerData.x += ROOM_TRANSITION_SPEED / 2;
    
            if (!newBlock)
            {
                gCamera.right++;
                gBackgroundInfo.blockX++;
            }
        }
        else if (gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
        {
            gBackgroundInfo.x -= ROOM_TRANSITION_SPEED;
            gPlayerData.x -= ROOM_TRANSITION_SPEED / 2;
    
            if (!newBlock)
            {
                gCamera.left--;
                gBackgroundInfo.blockX--;
            }
        }
    
        if (!newBlock)
            SetupTilemapUpdateX(gDoorTransition.direction);

        if (gDoorTransition.timer == SCREEN_SIZE_X_SUB_PIXEL / ROOM_TRANSITION_SPEED)
            gDoorTransition.stage = TRANSITION_STAGE_NORMAL_LAST_UPDATE;
    }
    else if (gDoorTransition.stage == TRANSITION_STAGE_NORMAL_LAST_UPDATE)
    {
        if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
        {
            gBackgroundInfo.blockX++;
            SetupTilemapUpdateX(TILEMAP_UPDATE_RIGHT);
        }
        else if (gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
        {
            gBackgroundInfo.blockX--;
            SetupTilemapUpdateX(TILEMAP_UPDATE_LEFT);
        }

        gDoorTransition.stage = TRANSITION_STAGE_NORMAL_ENDING;
    }
    else
    {
        gGameMode.main = GM_IN_GAME;
        gDoorTransition.type = TRANSITION_STAGE_NORMAL_NONE;

        if (gDoorTransition.direction == TILEMAP_UPDATE_RIGHT)
        {
            gCamera.x = 0;
            gCamera.y = 0;
        }
        else if (gDoorTransition.direction == TILEMAP_UPDATE_LEFT)
        {
            gCamera.x = gTilemap.width * BLOCK_SIZE - SCREEN_SIZE_X_SUB_PIXEL;
            gCamera.y = 0;
        }

        gCamera.top = 0;
        gCamera.bottom = SCREEN_SIZE_Y_BLOCK + 1;
    }
}
