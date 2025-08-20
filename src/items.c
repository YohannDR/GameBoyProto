#include "items.h"

#include "bg_clip.h"
#include "player.h"
#include "input.h"
#include "sprites_AI/water_drop.h"

#define ITEM_INPUT (KEY_B)

#define OFFSET_X_LEFT (-(QUARTER_BLOCK_SIZE))
#define OFFSET_X_RIGHT (PLAYER_WIDTH - QUARTER_BLOCK_SIZE)

#define OFFSET_Y (-(BLOCK_SIZE))

static u16 gItemX;
static u16 gItemY;
static u8 gDrawItem;

enum ItemState {
    STATE_IDLE,
    STATE_IN_USE,
};

static void Torch(void)
{

}

static void WaterBucket(void)
{
    if (gChangedInput & ITEM_INPUT)
    {
        // TODO, find a way to load graphics for dynamic sprites
        SpawnSprite(gItemX, gItemY, STYPE_WATER_DROP, WATER_DROP_FALLING, QueueSpriteGraphics(STYPE_WATER_DROP));
    }
}

static void AccessCard(void)
{

}

static void WaterGun(void)
{
    u8 direction;

    if (gChangedInput & ITEM_INPUT)
    {
        if (gPlayerMovement.direction & KEY_LEFT)
            direction = WATER_DROP_FLYING_LEFT;
        else
            direction = WATER_DROP_FLYING_RIGHT;

        // TODO, find a way to load graphics for dynamic sprites
        SpawnSprite(gItemX, gItemY, STYPE_WATER_DROP, direction, QueueSpriteGraphics(STYPE_WATER_DROP));
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

    gItemX = gPlayerData.x;
    if (gPlayerMovement.direction & KEY_RIGHT)
        gItemX += OFFSET_X_RIGHT;
    else
        gItemX += OFFSET_X_LEFT;

    gItemY = gPlayerData.y + OFFSET_Y;

    if (gPlayerData.pose == PLAYER_POSE_IDLE)

    sItemshandlers[gCurrentItem]();
}