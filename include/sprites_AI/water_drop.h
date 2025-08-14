#ifndef WATER_DROP_H
#define WATER_DROP_H

#include "types.h"

enum WaterDropPart {
    WATER_DROP_FALLING,
    WATER_DROP_FLYING_LEFT,
    WATER_DROP_FLYING_RIGHT
};

extern const u8 sWaterDropGraphics[];

void WaterDrop(void);

#endif /* WATER_DROP_H */
