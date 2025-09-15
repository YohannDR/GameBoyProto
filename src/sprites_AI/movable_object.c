#include "sprites_AI/movable_object.h"

#include "gb/oam.h"

#include "bg_clip.h"
#include "input.h"
#include "player.h"
#include "sprite.h"

enum MovableObjectPose {
    POSE_IDLE = 1,
    POSE_CARRIED,
    POSE_DROPPED,
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

    if (!(gChangedInput & INTERACTION_BUTTON))
        return;

    // Move down
    gCurrentSprite.y += BLOCK_SIZE;

    // Move in front of the player
    if (gPlayerMovement.direction & KEY_LEFT)
        gCurrentSprite.x -= BLOCK_SIZE + HALF_BLOCK_SIZE;
    else
        gCurrentSprite.x += BLOCK_SIZE + HALF_BLOCK_SIZE;
        
    // Check if the object ended up in a wall, we push it out if yes
    if (gPlayerMovement.direction & KEY_LEFT)
    {
        if (GET_CLIPDATA_SOLIDITY(gCurrentSprite.x + BLOCK_SIZE - HALF_BLOCK_SIZE, gCurrentSprite.y))
            gCurrentSprite.x = gCollisionInfo.right - BLOCK_SIZE + HALF_BLOCK_SIZE;
    }
    else
    {
        if (GET_CLIPDATA_SOLIDITY(gCurrentSprite.x + BLOCK_SIZE + HALF_BLOCK_SIZE, gCurrentSprite.y))
            gCurrentSprite.x = gCollisionInfo.left - BLOCK_SIZE - HALF_BLOCK_SIZE;
    }

    gPlayerData.carryingObject = FALSE;

    gCurrentSprite.pose = POSE_DROPPED;
}

void MovableObjectDropped(void)
{
    gCurrentSprite.y += QUARTER_BLOCK_SIZE;

    GetClipdataValue(gCurrentSprite.x + BLOCK_SIZE, gCurrentSprite.y + HALF_BLOCK_SIZE);

    if (gCollisionInfo.solidity != COLLISION_SOLID)
        return;

    gCurrentSprite.y = gCollisionInfo.top - HALF_BLOCK_SIZE;

    if (gCollisionInfo.behavior == CLIP_BEHAVIOR_RECEPTACLE)
    {
        gCurrentSprite.x &= BLOCK_POSITION_FLAG;
        gCurrentSprite.x += HALF_BLOCK_SIZE;

        gCurrentSprite.pose = POSE_LOCKED;
        gCurrentSprite.status |= SPRITE_STATUS_DISABLED;
    }
    else
    {
        gCurrentSprite.pose = POSE_IDLE;
    }
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
    else if (gCurrentSprite.pose == POSE_DROPPED)
        MovableObjectDropped();
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
