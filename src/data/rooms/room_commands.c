#include "data/rooms/room_commands.h"

#include "sprite.h"
#include "time_command.h"

static const u8 sRoomCommand_Empty[] = {
    TIME_COMMAND_END
};

static const u8 sRoomCommand_0[] = {
    MAKE_TILE_COMMAND(15, 15, 2, 5, 24),
    MAKE_SPRITE_COMMAND(15, 15, STYPE_FIRE_SPAWNER, (1 << 4) | 4),
    TIME_COMMAND_END
};

const u8* const sRoomCommands[] = {
    [0]  = sRoomCommand_0,
    [1]  = sRoomCommand_Empty,
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
