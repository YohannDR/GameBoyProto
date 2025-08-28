#include "time_command.h"

#include "bg_clip.h"
#include "sprite.h"

static const u8* ApplyTileCommand(const u8* commands)
{
    u8 tileValue;
    u8 width;
    u8 heigth;
    u8 x;
    u8 y;
    u8 i;
    u8 j;

    x = *commands++;
    y = *commands++;
    width = *commands++;
    heigth = *commands++;
    tileValue = *commands++;

    for (i = 0; i < heigth; i++)
    {
        for (j = 0; j < width; j++)
        {
            SetBgValueTile(x + j, y + i, tileValue);
        }
    }

    return commands;
}

static const u8* ApplySpriteCommand(const u8* commands)
{
    u8 x;
    u8 y;
    u8 spriteId;
    u8 spritePart;

    x = *commands++;
    y = *commands++;
    spriteId = *commands++;
    spritePart = *commands++;

    SpawnSprite(BLOCK_TO_SUB_PIXEL((u8)(x + 1)), BLOCK_TO_SUB_PIXEL((u8)(y + 2)), spriteId, spritePart, QueueSpriteGraphics(spriteId));

    return commands;
}

void ApplyTimeCommands(const u8* commands)
{
    u8 type;

    for (;;)
    {
        type = *commands++;

        if (type == TIME_COMMAND_END)
        {
            // We have no more commands to process, we can exit
            break;
        }

        if (type == TIME_COMMAND_CHANGE_TILES)
            commands = ApplyTileCommand(commands);
        else if (type == TIME_COMMAND_SPAWN_SPRITE)
            commands = ApplySpriteCommand(commands);
    }
}
