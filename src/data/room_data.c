#include "data/room_data.h"

#include "gb/display.h"

#include "sprite.h"

#include "data/rooms.h"

const struct RoomInfo sRooms[] = {
    [0] = {
        .tilemap = sRoom0_Tilemap,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom0_SpriteData,
        .doorData = sRoom0_DoorData,
        .collisionTable = 1,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 0,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 0,
    },
    [1] = {
        .tilemap = sRoom1_Tilemap,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom1_SpriteData,
        .doorData = sRoom1_DoorData,
        .collisionTable = 1,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 0,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 0,
    },
};
