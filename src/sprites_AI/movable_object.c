#include "sprites_AI/movable_object.h"

#include "gb/oam.h"

#include "input.h"
#include "player.h"
#include "sprite.h"

enum MovableObjectPose {
    POSE_IDLE = 1,
    POSE_CARRIED,
    POSE_LOCKED
};

extern const struct AnimData sMovableObjectAnim[];

static void MovableObjectIdle(void)
{
    if (gPlayerData.carryingObject)
        return;

    if (!(gCurrentSprite.status & SPRITE_STATUS_ON_SCREEN))
        return;

    if (!(gChangedInput & INTERACTION_BUTTON))
        return;

    if (gPlayerData.x + PLAYER_WIDTH / 2 < gCurrentSprite.x - HALF_BLOCK_SIZE)
        return;

    if (gPlayerData.x > gCurrentSprite.x + BLOCK_SIZE)
        return;

    if (gPlayerData.y < gCurrentSprite.y - HALF_BLOCK_SIZE)
        return;

    if (gPlayerData.y - PLAYER_HEIGHT > gCurrentSprite.y + HALF_BLOCK_SIZE)
        return;

    gCurrentSprite.pose = POSE_CARRIED;
    gPlayerData.carryingObject = TRUE;
}

static void MovableObjectCarried(void)
{
    gCurrentSprite.x = gPlayerData.x;
    gCurrentSprite.y = gPlayerData.y - (PLAYER_HEIGHT + HALF_BLOCK_SIZE);
}

void MovableObject(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sMovableObjectAnim;

        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);

        gCurrentSprite.x -= HALF_BLOCK_SIZE;
        gCurrentSprite.y -= HALF_BLOCK_SIZE;

        gCurrentSprite.pose = POSE_IDLE;
    }

    if (gCurrentSprite.pose == POSE_IDLE)
        MovableObjectIdle();
    else if (gCurrentSprite.pose == POSE_CARRIED)
        MovableObjectCarried();
}

static const u8 sMovableObjectAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-4), OAM_POS(4), 0, 0,
};

const struct AnimData sMovableObjectAnim[] = {
    [0] = {
        .oamPointer = sMovableObjectAnim_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sMovableObjectGraphics[] = {
    1,

    0xFF, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0xFF, 0xFF,
};
