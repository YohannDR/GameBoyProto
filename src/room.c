#include "room.h"
#include "bg.h"
#include "bg_clip.h"
#include "fading.h"
#include "sprite.h"
#include "callbacks.h"
#include "io.h"

#include "gb/io.h"

#include "data/room_data.h"
#include "data/tilesets.h"
#include "data/collision_tables.h"
#include "data/doors.h"

u8 gCurrentRoom;
u8 gCurrentTileset;
u16 gRoomOriginX;
u16 gRoomOriginY;
u16 gPreviousRoomOriginX;
u16 gPreviousRoomOriginY;

static void LoadSprites(const struct RoomSprite* sprites)
{
    u16 x;
    u16 y;
    u8 id;
    u8 part;

    PrepareSpriteGraphicsLoading();

    for (;;)
    {
        if (sprites->x == UCHAR_MAX)
            break;

        x = (u8)(sprites->x + 1) * BLOCK_SIZE;
        y = (u8)(sprites->y + 2) * BLOCK_SIZE;
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

void LoadRoom(u8 room)
{
    const struct RoomInfo* roomInfo;

    gCurrentRoom = room;

    roomInfo = &sRooms[gCurrentRoom];
    gBackgroundPalette = roomInfo->bgPalette;
    Write8(REG_BGP, gBackgroundPalette);

    gObj0Palette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK);
    gObj1Palette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK);

    Write8(REG_OBP0, gObj0Palette);
    Write8(REG_OBP1, gObj1Palette);

    LoadDoors(roomInfo->doorData);
    gCurrentCollisionTable = sCollisionTables[roomInfo->collisionTable];
    gPreviousRoomOriginX = gRoomOriginX;
    gPreviousRoomOriginY = gRoomOriginY;
    gRoomOriginX = roomInfo->originX;
    gRoomOriginY = roomInfo->originY;

    LoadSprites(roomInfo->spriteData);

    LoadTilemap(roomInfo->tilemap);
}

void TransitionToRoom(u8 room)
{
    const struct RoomInfo* roomInfo;

    gCurrentRoom = room;

    roomInfo = &sRooms[gCurrentRoom];

    gCurrentCollisionTable = sCollisionTables[roomInfo->collisionTable];
    gPreviousRoomOriginX = gRoomOriginX;
    gPreviousRoomOriginY = gRoomOriginY;
    gRoomOriginX = roomInfo->originX;
    gRoomOriginY = roomInfo->originY;
    BindTilemap(roomInfo->tilemap);
    LoadDoors(roomInfo->doorData);

    // LoadSprites(roomInfo->spriteData);
}
