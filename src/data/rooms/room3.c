#include "data/rooms/room3.h"

const u8 sRoom3_Tilemap[] = {
    80, 20,

    0xF0, 0x0A,
    0x50, 0x09,
    0x00, 0x00,
};

const struct RoomSprite sRoom3_SpriteData[] = {
    [0] = ROOM_SPRITE_TERMINATOR
};

const u8 sRoom3_DoorData[] = {
    7,
    8,
    9,
    10,
    DOOR_NONE
};
