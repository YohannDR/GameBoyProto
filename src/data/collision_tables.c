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
    [12] = CLIPDATA_AIR,
    [13] = CLIPDATA_AIR,
    [14] = CLIPDATA_AIR,
    [15] = CLIPDATA_AIR,
    [16] = CLIPDATA_AIR,
    [17] = CLIPDATA_AIR,
    [18] = CLIPDATA_AIR,
    [19] = CLIPDATA_AIR,
    [20] = CLIPDATA_AIR,
    [21] = CLIPDATA_AIR,
    [22] = CLIPDATA_AIR,
    [23] = CLIPDATA_AIR,
    [24] = CLIPDATA_AIR,
    [25] = CLIPDATA_AIR,
};

const u8* const sCollisionTables[] = {
    [0] = sCollisionTable_Test
};
