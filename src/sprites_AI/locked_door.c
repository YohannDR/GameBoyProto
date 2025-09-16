#include "sprites_AI/locked_door.h"

#include "gb/oam.h"

#include "bg_clip.h"
#include "sprite.h"

#define LOCKED_DOOR_DISAPPEARING_SPEED (QUARTER_BLOCK_SIZE * DELTA_TIME)

enum DoorLockPose {
    DOOR_LOCK_POSE_IDLE = 1,
    DOOR_LOCK_POSE_UNLOCKING,
    DOOR_LOCK_POSE_UNLOCKED,
};

extern const struct AnimData sLockedDoor_Anim[];

static void LockedDoorSetCollision(u8 tile)
{
    u8 x;
    u8 y;

    x = SUB_PIXEL_TO_BLOCK(gCurrentSprite.x) - 1;
    y = SUB_PIXEL_TO_BLOCK(gCurrentSprite.y) - 2;

    SetBgValueTile(x, y + 0, tile);
    SetBgValueTile(x, y + 1, tile);
    SetBgValueTile(x, y + 2, tile);
    SetBgValueTile(x, y + 3, tile);
}

static void LockedDoorSpawnLocks(void)
{
    u8 i;
    u16 y;

    y = gCurrentSprite.y;

    for (i = 0; i < gCurrentSprite.part; i++)
    {
        // Definitely not hacky
        (&gCurrentSprite.work1)[i] = SpawnSprite(gCurrentSprite.x, y, STYPE_DOOR_LOCK, 0, gCurrentSprite.gfxSlot);
        y += BLOCK_SIZE;
    }

    // Use ram slot as a lock counter, weird but all the work variables are already used
    gCurrentSprite.ramSlot = 0;
}

static void LockedDoorUnlockingInit(void)
{
    gCurrentSprite.pose = LOCKED_DOOR_POSE_UNLOCKING;
    gCurrentSprite.work1 = 0;
    
    LockedDoorSetCollision(0);
}

static void LockedDoorUnlockLock(void)
{
    u8 lockSlot;

    lockSlot = (&gCurrentSprite.work1)[gCurrentSprite.ramSlot];

    gSpriteData[lockSlot].pose = DOOR_LOCK_POSE_UNLOCKING;

    gCurrentSprite.ramSlot++;
    if (gCurrentSprite.ramSlot == gCurrentSprite.part)
        LockedDoorUnlockingInit();
    else
        gCurrentSprite.pose = LOCKED_DOOR_POSE_IDLE;
}

static void LockedDoorUnlocking(void)
{
    gCurrentSprite.y -= LOCKED_DOOR_DISAPPEARING_SPEED;

    gCurrentSprite.work1 += DELTA_TIME;

    if (gCurrentSprite.work1 >= BLOCK_SIZE * 4 / LOCKED_DOOR_DISAPPEARING_SPEED)
        gCurrentSprite.status = 0;
}

void LockedDoor(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sLockedDoor_Anim;

        gCurrentSprite.y -= BLOCK_SIZE;

        gCurrentSprite.drawDistanceTop = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE * 4);
        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);

        gCurrentSprite.pose = LOCKED_DOOR_POSE_IDLE;

        LockedDoorSpawnLocks();
        LockedDoorSetCollision(16);
    }

    if (gCurrentSprite.pose == LOCKED_DOOR_POSE_UNLOCK_LOCK)
        LockedDoorUnlockLock();
    else if (gCurrentSprite.pose == LOCKED_DOOR_POSE_UNLOCKING)
        LockedDoorUnlocking();
}

void DoorLock(void)
{
    gCurrentSprite.status |= SPRITE_STATUS_NOT_DRAWN;
}

static const u8 sLockedDoor_Anim_Frame0[OAM_DATA_SIZE(4)] = {
    4,
    OAM_POS(0), OAM_POS(0), 0, 0,
    OAM_POS(8), OAM_POS(0), 0, 0,
    OAM_POS(16), OAM_POS(0), 0, 0,
    OAM_POS(24), OAM_POS(0), 0, 0,
};

const struct AnimData sLockedDoor_Anim[] = {
    [0] = {
        .oamPointer = sLockedDoor_Anim_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sLockedDoorGraphics[] = {
    1,

    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
