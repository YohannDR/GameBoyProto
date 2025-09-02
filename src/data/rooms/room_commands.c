#include "data/rooms/room_commands.h"

#include "sprite.h"
#include "time_command.h"

static const u8 sRoomCommand_Empty[] = {
    TIME_COMMAND_END
};

static const u8 sRoomCommand_1[] = {
    MAKE_TILE_COMMAND(15, 17, 2, 4, 71),
    MAKE_TILE_COMMAND(15, 22, 1, 1, 28),
    MAKE_TILE_COMMAND(16, 22, 1, 1, 27),
    MAKE_FIRE_COMMAND(13, 14, 1 << 4, 5),
    MAKE_SPRITE_COMMAND(10, 35, STYPE_MOVABLE_OBJECT, 0),
    MAKE_SPRITE_COMMAND(31, 7, STYPE_MOVABLE_OBJECT, 0),
    MAKE_SPRITE_COMMAND(25, 15, STYPE_MOVABLE_OBJECT, 0),
    TIME_COMMAND_END
};

const u8* const sRoomCommands[] = {
    [0]  = sRoomCommand_Empty,
    [1]  = sRoomCommand_1,
    [2]  = sRoomCommand_Empty,
    [3]  = sRoomCommand_Empty,
    [4]  = sRoomCommand_Empty,
    [5]  = sRoomCommand_Empty,
    [6]  = sRoomCommand_Empty,
    [7]  = sRoomCommand_Empty,
    [8]  = sRoomCommand_Empty,
    [9]  = sRoomCommand_Empty,
    [10] = sRoomCommand_Empty,
    [11] = sRoomCommand_Empty,
    [12] = sRoomCommand_Empty,
    [13] = sRoomCommand_Empty,
    [14] = sRoomCommand_Empty,
    [15] = sRoomCommand_Empty,
    [16] = sRoomCommand_Empty,
    [17] = sRoomCommand_Empty,
    [18] = sRoomCommand_Empty,
    [19] = sRoomCommand_Empty,
    [20] = sRoomCommand_Empty,
    [21] = sRoomCommand_Empty,
    [22] = sRoomCommand_Empty,
    [23] = sRoomCommand_Empty,
    [24] = sRoomCommand_Empty,
    [25] = sRoomCommand_Empty,
    [26] = sRoomCommand_Empty,
    [27] = sRoomCommand_Empty,
    [28] = sRoomCommand_Empty,
};
