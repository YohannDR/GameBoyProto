#ifndef ROOM_H
#define ROOM_H

#include "types.h"
#include "sprite.h"
#include "bg_clip.h"
#include "door.h"

struct RoomSprite {
    u8 x;
    u8 y;
    u8 id;
    u8 part;
};

struct RoomInfo {
    const u8* tilemap;
    u8 bgPalette;
    const struct RoomSprite* spriteData;
    const u8* doorData;
    u8 collisionTable;
};

#define ROOM_SPRITE_TERMINATOR { .x = UCHAR_MAX, .y = UCHAR_MAX, .id = STYPE_NONE, .part = 0 }

extern u8 gCurrentRoom;
extern u8 gCurrentTileset;

void LoadRoom(u8 room);
ASM_IMPL void LoadGraphics(const u8* graphics);

#endif /* ROOM_H */
