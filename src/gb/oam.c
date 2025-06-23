#include "gb/oam.h"

u8 gNextOamSlot;

void ClearAndResetOam(void)
{
    u8 i;
    u8* oam;

    // Use a pointer instead of array indexing, it's much faster
    oam = gOamBuffer;

    for (i = 0; i < gNextOamSlot; i++)
    {
        *oam = 0;
        oam += 4;
    }

    gNextOamSlot = 0;
}
