#include "time_command.h"

#include "bg.h"
#include "bg_clip.h"
#include "fire.h"
#include "sprite.h"

struct TileChange {
    u8 x;
    u8 y;
    u8 width;
    u8 heigth;
    u8 tiles[8][8];
};

struct TileChange gTileChanges[4];
static u8 gTileChangesIndex;

static u8 gSpawnedSprites[10];
static u8 gSpawnedSpritesIndex;

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

    gTileChanges[gTileChangesIndex].x = x;
    gTileChanges[gTileChangesIndex].y = y;
    gTileChanges[gTileChangesIndex].width = width;
    gTileChanges[gTileChangesIndex].heigth = heigth;

    for (i = 0; i < heigth; i++)
    {
        for (j = 0; j < width; j++)
        {
            gTileChanges[gTileChangesIndex].tiles[i][j] = gDecompressedTilemap[(y + i) * gTilemap.width + (x + j)];
            SetBgValueTile(x + j, y + i, tileValue);
        }
    }

    gTileChangesIndex++;

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

    gSpawnedSprites[gSpawnedSpritesIndex] = SpawnSprite(BLOCK_TO_SUB_PIXEL((u8)(x + 1)), BLOCK_TO_SUB_PIXEL((u8)(y + 2)), spriteId, spritePart, QueueSpriteGraphics(spriteId));
    gSpawnedSpritesIndex++;

    return commands;
}

static const u8* ApplyFireCommand(const u8* commands)
{
    u8 x;
    u8 y;
    u8 data;

    x = *commands++;
    y = *commands++;
    data = *commands++;

    SpawnCluster(BLOCK_TO_SUB_PIXEL((u8)(x + 1)), BLOCK_TO_SUB_PIXEL((u8)(y + 2)), GET_UPPER_NIBBLE(data), GET_LOWER_NIBBLE(data));

    return commands;
}

static void RevertTileCommands(void)
{
    u8 i;
    u8 j;
    u8 k;

    for (i = 0; i < gTileChangesIndex; i++)
    {
        for (j = 0; j < gTileChanges[i].heigth; j++)
        {
            for (k = 0; k < gTileChanges[i].width; k++)
            {
                SetBgValueTile(gTileChanges[i].x + k, gTileChanges[i].y + j, gTileChanges[i].tiles[j][k]);
            }
        }
    }
}

static void RevertSpriteCommand(void)
{
    u8 i;

    // Just kill every spawned sprite
    for (i = 0; i < gSpawnedSpritesIndex; i++)
        gSpriteData[gSpawnedSprites[i]].status = 0;
}

void ApplyTimeCommands(const u8* commands)
{
    u8 type;

    gSpawnedSpritesIndex = 0;
    gTileChangesIndex = 0;

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
        else if (type == TIME_COMMAND_SPAWN_FIRE)
            commands = ApplyFireCommand(commands);
    }
}

void RevertTimeCommands(void)
{
    RevertTileCommands();
    RevertSpriteCommand();
}
