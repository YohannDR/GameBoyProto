#include "data/room_data.h"

#include "gb/display.h"

#include "sprite.h"

#include "data/rooms/room0.h"

const struct RoomInfo sRooms[] = {
    [0] = {
        .tilemap = sRoom0_Tilemap,
        .clipdata = sRoom0_Clipdata,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom0_SpriteData,
        .doorData = sRoom0_DoorData
    },
    [1] = {
        .tilemap = sRoom1_Tilemap,
        .clipdata = sRoom1_Clipdata,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom1_SpriteData,
        .doorData = sRoom1_DoorData
    }
};
