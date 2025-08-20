#ifndef INVENTORY_H
#define INVENTORY_H

#include "types.h"

/**
 * @brief List of acquirable items, it's split into 2 sub categories, selectable items and un-selectable items
 * 
 */
enum Item {
    ITEM_TORCH,
    ITEM_WATER_BUCKET,
    ITEM_ACCESS_CARD,
    ITEM_WATER_GUN,

    ITEM_SELECTABLE_END,

    ITEM_KEY_1 = ITEM_SELECTABLE_END,
    ITEM_KEY_2,
    ITEM_KEY_3,
    ITEM_KEY_4,

    ITEM_END,

    ITEM_NONE = UCHAR_MAX
};

enum InventoryState {
    INV_STATE_SCROLLING_UP,
    INV_STATE_SELECTING,
    INV_STATE_SCROLLING_DOWN,
};

extern u8 gCurrentItem;
extern u8 gAcquiredItems;

void InitializeWindow(void);

void OpenInventoryForNewItem(u8 item);
void OpenInventory(void);
void RegisterItem(u8 item);
u8 HasItem(u8 item);

void InventoryUpdate(void);

#endif /* INVENTORY_H */
