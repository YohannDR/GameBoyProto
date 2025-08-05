#include "data/collision_tables.h"
#include "bg_clip.h"

static const u8 sCollisionTable_Test[] = {
    [0] = CLIPDATA_SOLID,
    [1] = CLIPDATA_SOLID,
    [2] = CLIPDATA_SOLID,
    [3] = CLIPDATA_SOLID,
    [4] = CLIPDATA_SOLID,
    [5] = CLIPDATA_SOLID,
    [6] = CLIPDATA_SOLID,
    [7] = CLIPDATA_AIR,
    [8] = CLIPDATA_SOLID,
    [9] = CLIPDATA_SOLID,
    [10] = CLIPDATA_SOLID,
    [11] = CLIPDATA_SOLID,
    [12] = CLIPDATA_SOLID,
    [13] = CLIPDATA_SOLID,
    [14] = CLIPDATA_SOLID,
    [15] = CLIPDATA_SOLID,
    [16] = CLIPDATA_SOLID,
    [17] = CLIPDATA_SOLID,
    [18] = CLIPDATA_SOLID,
    [19] = CLIPDATA_SOLID,
    [20] = CLIPDATA_SOLID,
    [21] = CLIPDATA_SOLID,
    [22] = CLIPDATA_SOLID,
    [23] = CLIPDATA_SOLID,
    [24] = CLIPDATA_SOLID,
    [25] = CLIPDATA_SOLID,
};

const u8* const sCollisionTables[] = {
    [0] = sCollisionTable_Test
};
