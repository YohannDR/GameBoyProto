#include "data/sprite_data.h"

#include "sprites_AI/player.h"

static void Dummy(void) {}

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_PLAYER] = Player
};
