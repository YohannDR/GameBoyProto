#include "items.h"

#include "bg_clip.h"
#include "player.h"
#include "input.h"
#include "sprites_AI/water_drop.h"

#define ITEM_INPUT (KEY_B)

#define OFFSET_X_LEFT (0)
#define OFFSET_X_RIGHT (PLAYER_WIDTH - QUARTER_BLOCK_SIZE)

#define OFFSET_Y (-(BLOCK_SIZE))

struct ItemInfo {
    u8 state;
    u8 drawn;
    u8 work1;
};

static struct ItemInfo gItemInfo;
static u16 gItemX;
static u16 gItemY;

enum ItemState {
    STATE_IDLE,
    STATE_IN_USE,
};

static void Torch(void)
{

}

static void WaterBucket(void)
{

}

static void AccessCard(void)
{

}

static void WaterGun(void)
{
    u8 direction;

    if (gItemInfo.state == STATE_IDLE)
    {
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