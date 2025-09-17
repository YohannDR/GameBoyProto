#include "time_command.h"

#include "bank.h"
#include "bg.h"
#include "bg_clip.h"
#include "fire.h"
#include "sprite.h"
#include "time.h"

#include "data/sprite_data.h"

u8 gAlreadyTimeTravelled;

struct TileChange {
    u8 x;
    u8 y;
    u8 width;
    u8 heigth;
    u8 tiles[8][8];
};

static struct TileChange gTileChanges[4];
static u8 gTileChangesIndex;

static u8 gSpawnedSprites[10];
static u8 gSpawnedSpritesIndex;

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

void RevertSpriteCommand(void)
{
    u8 i;

    SwitchBank(BANK_SPRITE);

    for (i = 0; i < gSpawnedSpritesIndex; i++)
    {
        if (gCurrentTemporality == TEMPORALITY_PAST)
        {
            // Disable AI and rendering for the sprites
            gSpriteData[gSpawnedSprites[i]].status |= SPRITE_STATUS_DISABLED | SPRITE_STATUS_NOT_DRAWN;
        }
        else if (gCurrentTemporality == TEMPORALITY_FUTURE)
        {
            // Re-enable AI and rendering
            gSpriteData[gSpawnedSprites[i]].status &= ~(SPRITE_STATUS_DISABLED | SPRITE_STATUS_NOT_DRAWN);
        }

        gCurrentSprite = gSpriteData[gSpawnedSprites[i]];
        // Call sprite specific logic if necessary
        sSpriteTimeFuncPointers[gSpriteData[gSpawnedSprites[i]].type]();
        gSpriteData[gSpawnedSprites[i]] = gCurrentSprite;
    }

    BankGoBack();
}

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

    if (gAlreadyTimeTravelled)
    {
        RevertSpriteCommand();
    }
    else
    {
        gSpawnedSprites[gSpawnedSpritesIndex] = SpawnSprite(BLOCK_TO_SUB_PIXEL((u8)(x + 1)), BLOCK_TO_SUB_PIXEL((u8)(y + 2)), spriteId, spritePart, QueueSpriteGraphics(spriteId));
        gSpawnedSpritesIndex++;
    }

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

    if (!gAlreadyTimeTravelled)
    {
        SpawnCluster(BLOCK_TO_SUB_PIXEL((u8)(x + 1)), BLOCK_TO_SUB_PIXEL((u8)(y + 2)),
            GET_UPPER_NIBBLE(data), GET_LOWER_NIBBLE(data));
    }

    return commands;
}

void ApplyTimeCommands(const u8* commands)
{
    u8 type;

    if (!gAlreadyTimeTravelled)
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

    gAlreadyTimeTravelled = TRUE;
}

void RevertTimeCommands(void)
{
    RevertTileCommands();
    RevertSpriteCommand();
}
