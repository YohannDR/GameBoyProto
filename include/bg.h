#ifndef BG_H
#define BG_H

#include "types.h"
#include "macros.h"

struct TilemapInfo {
    const u8* tilemap;
    u8 width;
    u8 height;

    u8 left;
    u8 right;
    u8 top;
    u8 bottom;

    u8 reserved;
};

enum TilemapUpdateDirection {
    TILEMAP_UPDATE_NONE,
    TILEMAP_UPDATE_LEFT,
    TILEMAP_UPDATE_RIGHT,
    TILEMAP_UPDATE_TOP,
    TILEMAP_UPDATE_BOTTOM,
};

struct BackgroundInfo {
    u16 x;
    u16 y;
    u8 blockX;
    u8 blockY;
};

extern struct BackgroundInfo gBackgroundInfo;

extern u8 gWindowX;
extern u8 gWindowY;

extern struct TilemapInfo gTilemap;

extern u8 gTilemapUpdateDirection;

void LoadTilemap(const u8* tilemap);
void CheckForTilemapUpdate(void);
ASM_IMPL void TilemapUpdateVblank(void);

#endif /* BG_H */
