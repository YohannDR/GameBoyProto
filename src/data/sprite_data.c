#include "data/sprite_data.h"

#include "sprites_AI/player.h"

const SpriteFunc_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_PLAYER] = Player
};
