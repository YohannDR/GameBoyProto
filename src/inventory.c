#include "inventory.h"

#include "gb/io.h"

#include "bg.h"
#include "callbacks.h"
#include "io.h"
#include "input.h"
#include "items.h"
#include "macros.h"
#include "game_state.h"

#define INVENTORY_GFX_BASE_TILE (0xF0u)

u8 gCurrentItem;
u8 gAcquiredItems;

/**
 * @brief The items we can select, so the ones we have unlocked and none.
 * That's why we add 1 to to the size, it's to account for none since it's a separate thing
 * 
 */
static u8 gSelectableItems[ITEM_SELECTABLE_END + 1];

/**
 * @brief This amount of items we can scroll through, in other words, the effective size of the @c gSelectableItems array
 * 
 */
static u8 gNumberOfScrollableItems;

/**
 * @brief The tile of currently selected inventory item, written to VRAM during v-blank
 * 
 */
static u8 gInventoryItemTile;

/**
 * @brief The currently selected item in the array, not the same as the current item
 * 
 */
static u8 gSelectIndex;

static u8 gPreviousItem;

extern const u8 sWindowGraphics[];

static void InventoryVblank(void)
{
    // We only need to update the window position, since the game is paused, we can garantee the screen will not scroll
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);

    // Write the tile to VRAM
    Write8(VRAM_BASE + 0x1C00 + SCREEN_SIZE_X_BLOCK / 2, gInventoryItemTile);
}

static void SetupWindowTilemap(void)
{
    u8* dst;
    u8 i;

    dst = (u8*)(VRAM_BASE + 0x1C00);

    // Fill with white
    for (i = 0; i < 32 * 2; i++)
        *dst++ = INVENTORY_GFX_BASE_TILE + 7u;

    // Add the 2 arrows
    Write8(VRAM_BASE + 0x1C00 + SCREEN_SIZE_X_BLOCK / 2 - 1, INVENTORY_GFX_BASE_TILE + 0u);
    Write8(VRAM_BASE + 0x1C00 + SCREEN_SIZE_X_BLOCK / 2 + 1, INVENTORY_GFX_BASE_TILE + 1u);
}

static void SetupWindowGraphics(void)
{
    const u8* src;
    u8* dst;
    u8 tileCount;

    src = sWindowGraphics;
    tileCount = *src++ * 16;

    // Load the graphics at the end of the the shared VRAM space
    dst = (u8*)(VRAM_BASE + 0xF00);

    while (tileCount--)
        *dst++ = *src++;
}

static void ComputeInventoryTile(void)
{
    if (gCurrentItem == ITEM_NONE)
        gInventoryItemTile = INVENTORY_GFX_BASE_TILE + 7u;
    else
        gInventoryItemTile = INVENTORY_GFX_BASE_TILE + 3u + gCurrentItem;
}

static void ComputeSelectableItems(void)
{
    u8 i;
    u8 index;
    u8 items;

    gNumberOfScrollableItems = 0;
    index = 0;
    items = gAcquiredItems;

    for (i = 0; i < ITEM_SELECTABLE_END; i++)
    {
        // Check if we have the item
        if (items & 1)
        {
            // Add it
            gSelectableItems[index] = i;

            // While we're at it, check if it's our currently equipped item, we want to start the selection from here
            if (i == gCurrentItem)
                gSelectIndex = index;

            index++;
        }

        // Goto next item
        items >>= 1;
    }

    // Add the special "none" item
    gSelectableItems[index] = ITEM_NONE;
    gNumberOfScrollableItems = index + 1;

    // And check if that's our current item, since it's not covered in the loop
    if (gCurrentItem == ITEM_NONE)
        gSelectIndex = index;
}

void InitializeWindow(void)
{
    SetupWindowGraphics();
    SetupWindowTilemap();
}

void OpenInventoryForNewItem(u8 item)
{
    gGameMode.main = GM_INVENTORY;
    gGameMode.sub = INV_STATE_SCROLLING_UP;
    gGameMode.timer = 0;
    gGameMode.work1 = TRUE;
    gGameMode.work2 = 0;

    gPreviousItem = gCurrentItem;

    gCurrentItem = item;
    ComputeInventoryTile();

    CallbackSetVblank(InventoryVblank);
}

void OpenInventory(void)
{
    // Setup game mode
    gGameMode.main = GM_INVENTORY;
    gGameMode.sub = INV_STATE_SCROLLING_UP;
    gGameMode.work1 = FALSE;

    // Compute the tile once at startup to properly display it when the window is scrolling up
    ComputeInventoryTile();
    // Prepare the items
    ComputeSelectableItems();

    CallbackSetVblank(InventoryVblank);
}

void RegisterItem(u8 item)
{
    gAcquiredItems |= 1 << item;
}

u8 HasItem(u8 item)
{
    return gAcquiredItems & (1 << item);
}

void InventoryUpdate(void)
{
    if (gGameMode.sub == INV_STATE_SCROLLING_UP)
    {
        // Move upwards
        gWindowY -= 1;

        // Check if we reached a tile and one pixel visible
        if (gWindowY == SUB_PIXEL_TO_PIXEL(SCREEN_SIZE_Y_SUB_PIXEL - BLOCK_SIZE - PIXEL_SIZE))
            gGameMode.sub = INV_STATE_SELECTING;
    }
    else if (gGameMode.sub == INV_STATE_SELECTING)
    {
        if (gGameMode.work1)
        {
            gGameMode.timer += DELTA_TIME;

            if (gGameMode.timer == CONVERT_SECONDS(.4f))
            {
                gGameMode.timer = 0;
                gGameMode.work2++;

                // Alternate between correct tile and blank tile
                if (gInventoryItemTile == INVENTORY_GFX_BASE_TILE + 7u)
                    ComputeInventoryTile();
                else
                    gInventoryItemTile = INVENTORY_GFX_BASE_TILE + 7u;

                if (gGameMode.work2 == 5)
                {
                    gGameMode.sub = INV_STATE_SCROLLING_DOWN;
                    gCurrentItem = gPreviousItem;
                }
            }
        }
        else
        {
            // Check if we should exit the inventory, either B or Select works
            if (gChangedInput & (KEY_B | KEY_SELECT))
            {
                gGameMode.sub = INV_STATE_SCROLLING_DOWN;
                return;
            }
    
            // Handle scrolling, check for wrap around
            if (gChangedInput & KEY_LEFT)
            {
                if (gSelectIndex == 0)
                    gSelectIndex = gNumberOfScrollableItems - 1;
                else
                    gSelectIndex--;
            }
            else if (gChangedInput & KEY_RIGHT)
            {
                if (gSelectIndex == gNumberOfScrollableItems - 1)
                    gSelectIndex = 0;
                else
                    gSelectIndex++;
            }
    
            // Update current item with our selection
            gCurrentItem = gSelectableItems[gSelectIndex];
            OnItemSwitch();

            // And update the displayed tile
            ComputeInventoryTile();
        }

    }
    else if (gGameMode.sub == INV_STATE_SCROLLING_DOWN)
    {
        // Move downwards
        gWindowY += 1;

        // Check if we're outside of the screen, the reason we check for slightly below (+ PIXEL_SIZE)
        // is because we immediatly change the v-blank callback, so the window position we computed on the last frame will never be applied
        // hence why we stall for a single pixel/frame
        if (gWindowY == SUB_PIXEL_TO_PIXEL(SCREEN_SIZE_Y_SUB_PIXEL + PIXEL_SIZE))
        {
            gGameMode.main = GM_IN_GAME;

            // We set back the default v-blank
            CallbackSetVblank(VblankCallback);
        }
    }
}

const u8 sWindowGraphics[] = {
    8,

    0x00, 0x00, 0x08, 0x08, 0x10, 0x18, 0x20, 0x38, 0x40, 0x78, 0x20, 0x38, 0x10, 0x18, 0x08, 0x08,
    0x00, 0x00, 0x10, 0x10, 0x08, 0x18, 0x04, 0x1C, 0x02, 0x1E, 0x04, 0x1C, 0x08, 0x18, 0x10, 0x10,
    0x00, 0x00, 0x10, 0x10, 0x28, 0x38, 0x44, 0x7C, 0x82, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x04, 0x0A, 0x0E, 0x0A, 0x0E, 0x0C, 0x14, 0x10, 0x28, 0x20, 0x50, 0x40, 0xA0, 0x00, 0xC0,
    0xC3, 0xC3, 0xFF, 0xBD, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0xC3, 0x7E, 0x66, 0x3C, 0x3C,
    0x00, 0x7E, 0x00, 0x81, 0x1C, 0x9D, 0x20, 0xA1, 0x20, 0xA1, 0x1C, 0x9D, 0x00, 0x81, 0x00, 0x7E,
    0x00, 0x00, 0x80, 0x80, 0x7F, 0x7F, 0xC1, 0xFF, 0x7F, 0x5F, 0xF4, 0x94, 0xE8, 0xA8, 0x60, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
