#include "gb/oam.h"

u8 gNextOamSlot;

void ClearAndResetOam(void)
{
    u8 i;

    for (i = 0; i < gNextOamSlot; i++)
        gOamBuffer[i].y = 0;

    gNextOamSlot = 0;
}
