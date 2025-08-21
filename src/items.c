#include "items.h"

#include "bg_clip.h"
#include "player.h"
#include "input.h"
#include "fire.h"
#include "sprites_AI/water_drop.h"

#define ITEM_INPUT (KEY_B)

#define OFFSET_X_LEFT (-(QUARTER_BLOCK_SIZE))
#define OFFSET_X_RIGHT (PLAYER_WIDTH - QUARTER_BLOCK_SIZE)

#define OFFSET_Y (-(BLOCK_SIZE + HALF_BLOCK_SIZE))

static u16 gItemX;
static u16 gItemY;
static u8 gDrawItem;
static u8 gTempTimer;

/**
 * @brief Whether the current item is "on" : is the torch light up, is the bucket filled, is the gun filled
 * 
 */
static u8 gIsItemOn;

enum ItemState {
    STATE_IDLE,
    STATE_IN_USE,
};

static void CheckFillWithWater(void)
{
    if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x, gPlayerData.y) == CLIP_BEHAVIOR_WATER)
        gIsItemOn = TRUE;
    else if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH / 2, gPlayerData.y) == CLIP_BEHAVIOR_WATER)
        gIsItemOn = TRUE;
    else if (GET_CLIPDATA_BEHAVIOR(gPlayerData.x + PLAYER_WIDTH, gPlayerData.y) == CLIP_BEHAVIOR_WATER)
        gIsItemOn = TRUE;
}

static void Torch(void)
{
    u16 x;

    x = gItemX;
    if (gPlayerMovement.direction & KEY_RIGHT)
        x += BLOCK_SIZE;

    if ((gTempTimer % 32) == 0 && GET_CLIPDATA_BEHAVIOR(x, gItemY) == CLIP_BEHAVIOR_INFLAMMABLE)
    {
        StartFire(x, gItemY);
    }
}

static void WaterBucket(void)
{
    if (!gIsItemOn)
        CheckFillWithWater();

    if (gIsItemOn && gChangedInput & ITEM_INPUT)
    {
        // TODO, find a way to load graphics for dynamic sprites
        SpawnSprite(gItemX, gItemY, STYPE_WATER_DROP, WATER_DROP_FALLING, QueueSpriteGraphics(STYPE_WATER_DROP));

        gIsItemOn = FALSE;
    }
}

static void AccessCard(void)
{

}

static void WaterGun(void)
{
    u8 direction;

    if (!gIsItemOn)
        CheckFillWithWater();

    if (gIsItemOn && gChangedInput & ITEM_INPUT)
    {
        if (gPlayerMovement.direction & KEY_LEFT)
            direction = WATER_DROP_FLYING_LEFT;
        else
            direction = WATER_DROP_FLYING_RIGHT;

        // TODO, find a way to load graphics for dynamic sprites
        SpawnSprite(gItemX, gItemY, STYPE_WATER_DROP, direction, QueueSpriteGraphics(STYPE_WATER_DROP));

        gIsItemOn = FALSE;
    }
}

static const Func_T sItemshandlers[ITEM_SELECTABLE_END] = {
    [ITEM_TORCH] = Torch,
    [ITEM_WATER_BUCKET] = WaterBucket,
    [ITEM_ACCESS_CARD] = AccessCard,
    [ITEM_WATER_GUN] = WaterGun,
};

void UpdateItem(void)
{
    if (gCurrentItem == ITEM_NONE)
        return;

    gTempTimer++;

    gItemX = gPlayerData.x;
    if (gPlayerMovement.direction & KEY_RIGHT)
        gItemX += OFFSET_X_RIGHT;
    else
        gItemX += OFFSET_X_LEFT;

    gItemY = gPlayerData.y + OFFSET_Y;

    if (gPlayerData.pose == PLAYER_POSE_IDLE)

    sItemshandlers[gCurrentItem]();
}

void OnItemSwitch(void)
{
    gIsItemOn = FALSE;
}
