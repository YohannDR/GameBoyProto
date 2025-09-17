#include "room.h"

#include "gb/io.h"

#include "bank.h"
#include "bg.h"
#include "bg_clip.h"
#include "fading.h"
#include "sprite.h"
#include "fire.h"
#include "callbacks.h"
#include "time.h"
#include "time_command.h"
#include "io.h"

#include "data/room_data.h"
#include "data/tilesets.h"
#include "data/collision_tables.h"
#include "data/doors.h"
#include "data/rooms/room_commands.h"

u8 gCurrentRoom;
u8 gCurrentTileset;

static void LoadSprites(const struct RoomSprite* sprites)
{
    u16 x;
    u16 y;
    u8 id;
    u8 part;

    ClearSprites();

    PrepareSpriteGraphicsLoading();
    gMaxAmountOfExistingSprites = 0;

    for (;;)
    {
        if (sprites->x == UCHAR_MAX)
            break;

        x = BLOCK_TO_SUB_PIXEL((u8)(sprites->x + 1));
        y = BLOCK_TO_SUB_PIXEL((u8)(sprites->y + 2));
        id = sprites->id;
        part = sprites->part;
        SpawnSprite(x, y, id, part, QueueSpriteGraphics(id));
        sprites++;
    }

    StartSpriteGraphicsLoading();
}

static void LoadDoors(const u8* doorData)
{
    u8 doorId;

    DoorReset();

    for (;;)
    {
        doorId = *doorData++;

        if (doorId == DOOR_NONE)
            break;

        DoorLoad(&sDoors[doorId]);
    }
}

void LoadRoom(u8 room, u8 loadTilemap)
{
    const struct RoomInfo* roomInfo;

    gCurrentRoom = room;

    // Switch to data bank where everything is located
    SwitchBank(BANK_DATA);

    roomInfo = &sRooms[gCurrentRoom];
    gBackgroundPalette = roomInfo->bgPalette;

    if (loadTilemap)
        Write8(REG_BGP, gBackgroundPalette);

    LoadDoors(roomInfo->doorData);
    gCurrentCollisionTable = sCollisionTables[roomInfo->collisionTable];
    
    ClearFire();
    LoadTilemap(roomInfo->tilemap, loadTilemap);
    LoadSprites(roomInfo->spriteData);

    gAlreadyTimeTravelled = FALSE;

    if (gCurrentTemporality == TEMPORALITY_FUTURE)
        ApplyTimeCommands(sRoomCommands[gCurrentRoom]);
}
