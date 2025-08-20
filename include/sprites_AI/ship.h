#ifndef SHIP_H
#define SHIP_H

#include "types.h"

enum ShipType {
    SHIP_LEFT,
    SHIP_RIGHT,
};

extern const u8 sShipGraphics[];

void Ship(void);

#endif /* SHIP_H */
