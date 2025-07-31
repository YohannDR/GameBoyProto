#include "data/room_data.h"

#include "gb/display.h"

#include "sprite.h"

#include "data/rooms/room0.h"

const struct RoomInfo sRooms[] = {
    [0] = {
        .graphics = sRoom0_Graphics,
        .tilemap = sRoom0_Tilemap,
        .clipdata = sRoom0_Clipdata,
        .bgPalette = MAKE_PALETTE(COLOR_WHITE, COLOR_LIGHT_GRAY, COLOR_DARK_GRAY, COLOR_BLACK),
        .spriteData = sRoom0_SpriteData,
    },
};
