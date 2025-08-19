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
        .collisionTable = 0,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 0,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 13024,
    },
    [1] = {
        .tilemap = sRoom1_Tilemap,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom1_SpriteData,
        .doorData = sRoom1_DoorData,
        .collisionTable = 0,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 2,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 13024,
    },
    [2] = {
        .tilemap = sRoom2_Tilemap,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom2_SpriteData,
        .doorData = sRoom2_DoorData,
        .collisionTable = 0,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 0,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 0,
    },
    [3] = {
        .tilemap = sRoom3_Tilemap,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom3_SpriteData,
        .doorData = sRoom3_DoorData,
        .collisionTable = 0,
        .originX = SCREEN_SIZE_X_SUB_PIXEL * 0,
        .originY = SCREEN_SIZE_Y_SUB_PIXEL * 0,
    },
};
