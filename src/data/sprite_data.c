#include "data/sprite_data.h"

#include "sprites_AI/portal.h"
#include "sprites_AI/water_drop.h"
#include "sprites_AI/moving_platform.h"
#include "sprites_AI/collectible.h"
#include "sprites_AI/movable_object.h"
#include "sprites_AI/ship.h"
#include "sprites_AI/fire_spawner.h"

static void Dummy(void) {}

static const u8 sDummyGraphics[] = {
    0,
};

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_PORTAL] = Portal,
    [STYPE_SHIP] = Ship,
    [STYPE_WATER_DROP] = WaterDrop,
    [STYPE_MOVING_PLATFORM] = MovingPlatform,
    [STYPE_COLLECTIBLE] = Collectible,
    [STYPE_MOVABLE_OBJECT] = MovableObject,
    [STYPE_FIRE_SPAWNER] = FireSpawner,
};

const u8* const sSpriteGraphicsPointers[STYPE_END] = {
    [STYPE_NONE] = sDummyGraphics,
    [STYPE_PORTAL] = sPortalGraphics,
    [STYPE_SHIP] = sShipGraphics,
    [STYPE_WATER_DROP] = sWaterDropGraphics,
    [STYPE_MOVING_PLATFORM] = sMovingPlatformGraphics,
    [STYPE_COLLECTIBLE] = sCollectibleGraphics,
    [STYPE_MOVABLE_OBJECT] = sMovableObjectGraphics,
    [STYPE_FIRE_SPAWNER] = sDummyGraphics,
};
