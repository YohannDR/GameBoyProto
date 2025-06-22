#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"
#include "macros.h"

struct Oam {
    u8 y;
    u8 x;
    u8 tile;
    u8 attributes;
};

#define OAM_ATTR_OBP_0      (0u << 4)
#define OAM_ATTR_OBP_1      (1u << 4)
#define OAM_ATTR_X_FLIP     (1u << 5)
#define OAM_ATTR_Y_FLIP     (1u << 6)
#define OAM_ATTR_HIGH_PRIO  (0u << 7)
#define OAM_ATTR_LOW_PRIO   (1u << 7)

// Wram buffer for the Oam, mapped directly to shadow oam and automatically transfered to actual oam during v-blank
extern struct Oam gOamBuffer[40];

#endif /* SPRITE_H */
