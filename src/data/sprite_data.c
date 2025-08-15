#include "data/sprite_data.h"

#include "sprites_AI/fire.h"
#include "sprites_AI/portal.h"
#include "sprites_AI/water_drop.h"
#include "sprites_AI/moving_platform.h"

static void Dummy(void) {}

static const u8 sDummyGraphics[] = {
    0,
};

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_PORTAL] = Portal,
    [STYPE_FIRE] = Fire,
    [STYPE_SHIP] = Dummy,
    [STYPE_WATER_DROP] = WaterDrop,
    [STYPE_MOVING_PLATFORM] = MovingPlatform,
};

const u8* const sSpriteGraphicsPointers[STYPE_END] = {
    [STYPE_NONE] = sDummyGraphics,
    [STYPE_PORTAL] = sPortalGraphics,
    [STYPE_FIRE] = sFireGraphics,
    [STYPE_SHIP] = sDummyGraphics,
    [STYPE_WATER_DROP] = sWaterDropGraphics,
    [STYPE_MOVING_PLATFORM] = sMovingPlatformGraphics,
};
