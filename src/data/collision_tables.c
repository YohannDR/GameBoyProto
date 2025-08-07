#include "data/collision_tables.h"
#include "bg_clip.h"

static const u8 sCollisionTable_Test[] = {
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_AIR,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_SOLID,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
    CLIPDATA_AIR,
};

const u8* const sCollisionTables[] = {
    sCollisionTable_Test,
};
