#include "sprites_AI/locked_door.h"

#include "gb/oam.h"

#include "bg_clip.h"
#include "sprite.h"
#include "room.h"

enum DoorLockPose {
    DOOR_LOCK_POSE_IDLE = 1,
    DOOR_LOCK_POSE_UNLOCKING_INIT,
    DOOR_LOCK_POSE_UNLOCKING,
    DOOR_LOCK_POSE_UNLOCKED,
};

extern const struct AnimData sLockedDoorAnim[];

extern const struct AnimData sDoorLockAnim_Idle[];
extern const struct AnimData sDoorLockAnim_Unlocking[];
extern const struct AnimData sDoorLockAnim_Unlocked[];

/**
 * @brief Which doors have been permanently opened
 * 
 */
static u8 gOpenedDoors[4];

static u8 IsDoorOpened(void)
{
    u8 index;
    u8 bit;

    index = gCurrentRoom / 8;
    bit = 1 << (gCurrentRoom % 8);

    return gOpenedDoors[index] & bit;
}

static void SetDoorAsOpened(void)
{
    u8 index;
    u8 bit;

    index = gCurrentRoom / 8;
    bit = 1 << (gCurrentRoom % 8);

    gOpenedDoors[index] |= bit;
}

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
    u8 i;

    LockedDoorSetCollision(0);
    SetDoorAsOpened();

    for (i = 0; i < gCurrentSprite.part; i++)
        gSpriteData[(&gCurrentSprite.work1)[i]].status = 0;

    gCurrentSprite.status = 0;
}

static void LockedDoorUnlockLock(void)
{
    u8 lockSlot;

    lockSlot = (&gCurrentSprite.work1)[gCurrentSprite.ramSlot];

    gSpriteData[lockSlot].pose = DOOR_LOCK_POSE_UNLOCKING_INIT;
    gSpriteData[lockSlot].status &= ~SPRITE_STATUS_DISABLED;

    gCurrentSprite.ramSlot++;
    if (gCurrentSprite.ramSlot == gCurrentSprite.part)
        LockedDoorUnlockingInit();
    else
        gCurrentSprite.pose = LOCKED_DOOR_POSE_IDLE;
}

void LockedDoor(void)
{
    if (gCurrentSprite.pose == 0)
    {
        if (IsDoorOpened())
        {
            gCurrentSprite.status = 0;
            return;
        }

        gCurrentSprite.animPointer = sLockedDoorAnim;

        gCurrentSprite.y -= BLOCK_SIZE;

        gCurrentSprite.drawDistanceTop = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE * 4);
        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);

        gCurrentSprite.pose = LOCKED_DOOR_POSE_IDLE;

        LockedDoorSpawnLocks();
        LockedDoorSetCollision(DEFAULT_SOLID_TILE);
    }

    if (gCurrentSprite.pose == LOCKED_DOOR_POSE_UNLOCK_LOCK)
        LockedDoorUnlockLock();
}

void DoorLock(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sDoorLockAnim_Idle;

        gCurrentSprite.drawDistanceTop = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);

        gCurrentSprite.pose = DOOR_LOCK_POSE_IDLE;

        gCurrentSprite.status |= SPRITE_STATUS_DISABLED;
    }
    else if (gCurrentSprite.pose == DOOR_LOCK_POSE_UNLOCKING_INIT)
    {
        gCurrentSprite.animPointer = sDoorLockAnim_Unlocking;
        gCurrentSprite.animTimer = 0;

        gCurrentSprite.pose = DOOR_LOCK_POSE_UNLOCKING;
    }
    else if (gCurrentSprite.pose == DOOR_LOCK_POSE_UNLOCKING)
    {
        if (gCurrentSprite.status & SPRITE_STATUS_ANIM_ENDED)
        {
            gCurrentSprite.pose = DOOR_LOCK_POSE_UNLOCKED;

            gCurrentSprite.animPointer = sDoorLockAnim_Unlocked;
            gCurrentSprite.animTimer = 0;
            gCurrentSprite.currentAnimFrame = 0;

            gCurrentSprite.status |= SPRITE_STATUS_DISABLED;
        }
    }
}

static const u8 sLockedDoorAnim_Frame0[OAM_DATA_SIZE(4)] = {
    4,
    OAM_POS(0), OAM_POS(0), 0, 0,
    OAM_POS(8), OAM_POS(0), 0, 0,
    OAM_POS(16), OAM_POS(0), 0, 0,
    OAM_POS(24), OAM_POS(0), 0, 0,
};

const struct AnimData sLockedDoorAnim[] = {
    [0] = {
        .oamPointer = sLockedDoorAnim_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

static const u8 sDoorLockAnim_Idle_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(-1), 1, 0,
};

const struct AnimData sDoorLockAnim_Idle[] = {
    [0] = {
        .oamPointer = sDoorLockAnim_Idle_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

static const u8 sDoorLockAnim_Unlocking_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(-1), 3, 0,
};

const struct AnimData sDoorLockAnim_Unlocking[] = {
    [0] = {
        .oamPointer = sDoorLockAnim_Unlocking_Frame0,
        .duration = 60,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

static const u8 sDoorLockAnim_Unlocked_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(-1), 2, 0,
};

const struct AnimData sDoorLockAnim_Unlocked[] = {
    [0] = {
        .oamPointer = sDoorLockAnim_Unlocked_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sLockedDoorGraphics[] = {
    4,

    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
    0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E,
    0x00, 0x00, 0x18, 0x00, 0x3C, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x3C, 0x00, 0x18, 0x00, 0x00, 0x00,
};
