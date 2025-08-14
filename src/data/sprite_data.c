#include "data/sprite_data.h"

#include "sprites_AI/fire.h"
#include "sprites_AI/portal.h"

static void Dummy(void) {}

static const u8 sDummyGraphics[] = {
    0,
};

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_PORTAL] = Portal,
    [STYPE_FIRE] = Fire,
    [STYPE_DOOR] = Dummy,
    [STYPE_SHIP] = Dummy
};

const u8* const sSpriteGraphicsPointers[STYPE_END] = {
    [STYPE_NONE] = sDummyGraphics,
    [STYPE_PORTAL] = sPortalGraphics,
    [STYPE_FIRE] = sFireGraphics,
    [STYPE_DOOR] = sDummyGraphics,
    [STYPE_SHIP] = sDummyGraphics
};
