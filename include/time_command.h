#ifndef TIME_COMMAND_H
#define TIME_COMMAND_H

#include "types.h"

enum TimeCommandType {
    TIME_COMMAND_CHANGE_TILES,
    TIME_COMMAND_SPAWN_SPRITE,

    TIME_COMMAND_END,
};

#define MAKE_TILE_COMMAND(x, y, width, height, value) TIME_COMMAND_CHANGE_TILES, (x), (y), (width), (height), (value)
#define MAKE_SPRITE_COMMAND(x, y, spriteId, spritePart) TIME_COMMAND_SPAWN_SPRITE, (x), (y), (spriteId), (spritePart)

void ApplyTimeCommands(const u8* commands);

#endif /* TIME_COMMAND_H */
