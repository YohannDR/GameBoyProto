#include "door.h"

#include "bg.h"
#include "fading.h"
#include "macros.h"
#include "room.h"
#include "player.h"

#include "data/doors.h"

#define TRANSITION_SPEED 1

struct Door gDoors[4];
struct DoorTransition gDoorTransition;
static u8 gCurrentNumberOfDoors;

void DoorReset(void)
{
    gCurrentNumberOfDoors = 0;
}

void DoorLoad(const struct Door* door)
{
    gDoors[gCurrentNumberOfDoors] = *door;
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
        if (gPlayerData.x < door->x)
            continue;

        if (gPlayerData.y < door->y)
            continue;

        if (gPlayerData.x > door->x + door->width * BLOCK_SIZE)
            continue;

        if (gPlayerData.y > door->y + door->height * BLOCK_SIZE)
            continue;

        if (door->tileset != gCurrentTileset && door->tileset != UCHAR_MAX)
        {
            gDoorTransition.type = TRANSITION_TYPE_LOADING;
            FadingStart(FADING_TARGET_BACKGROUND | FADING_TARGET_OBJ0 | FADING_TARGET_OBJ1, PALETTE_BLACK, 10);
        }
        else
        {
            /*
            gDoorTransition.type = TRANSITION_TYPE_NORMAL;
            gDoorTransition.stage = TRANSITION_STAGE_NORMAL_SCROLLING;
            gDoorTransition.direction = door->targetDoor == 1 ? TILEMAP_UPDATE_RIGHT : TILEMAP_UPDATE_LEFT;
            gDoorTransition.timer = 0;
            TransitionToRoom(sDoors[door->targetDoor].ownerRoom);
            */
        }
    }
}
