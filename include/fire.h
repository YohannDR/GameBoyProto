#ifndef FIRE_H
#define FIRE_H

#include "types.h"

void LoadFireGraphics(void);
void StartFire(u16 x, u16 y);
void SpawnCluster(u16 x, u16 y, u8 direction, u8 length);
u8 IsTileBurned(u16 x, u16 y);
void UpdateFire(void);

#endif /* FIRE_H */
