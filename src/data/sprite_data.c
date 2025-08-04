#include "data/sprite_data.h"

static void Dummy(void) {}

static const u8 sDummyGraphics[] = {
    0,
};

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_DUMMY] = Dummy
};

const u8* const sSpriteGraphicsPointers[STYPE_END] = {
    [STYPE_NONE] = sDummyGraphics,
    [STYPE_DUMMY] = sDummyGraphics,
};
