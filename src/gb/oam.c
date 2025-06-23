#include "gb/oam.h"

u8 gNextOamSlot;

void ClearAndResetOam(void)
{
    u8 i;

    for (i = 0; i < gNextOamSlot; i++)
        gOamBuffer[(u8)(i * 4)] = 0;

    gNextOamSlot = 0;
}
