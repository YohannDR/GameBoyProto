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
#include "data/doors.h"

u8 gCurrentRoom;
u8 gCurrentTileset;

static void LoadSprites(const struct RoomSprite* sprites)
{
    u16 x;
    u16 y;
    u8 id;
    u8 part;

    for (;;)
    {
        if (sprites->x == UCHAR_MAX)
            break;

        x = (u8)(sprites->x + 1) * BLOCK_SIZE;
        y = (u8)(sprites->y + 2) * BLOCK_SIZE;
        id = sprites->id;
        part = sprites->part;
        SpawnSprite(x, y, id, part, LoadSpriteGraphics(id));
        sprites++;
    }
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

    LoadClipdata(roomInfo->clipdata);
    LoadDoors(roomInfo->doorData);
    // LoadSprites(roomInfo->spriteData);

    LoadTilemap(roomInfo->tilemap);

    // Enable display, background and objects
    Write8(REG_LCDC, LCDC_LCD_ENABLE | LCDC_BG_ENABLE | LCDC_OBJ_ENABLE);
}

void TransitionToRoom(u8 room)
{
    (void)room;
    /*
    const struct RoomInfo* roomInfo;

    gCurrentRoom = room;

    roomInfo = &sRooms[gCurrentRoom];

    LoadTilemapForTransition(roomInfo->tilemap);
    LoadClipdata(roomInfo->clipdata);
    LoadDoors(roomInfo->doorData);
    // LoadSprites(roomInfo->spriteData);
    */
}
