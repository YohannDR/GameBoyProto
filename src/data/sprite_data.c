#include "data/sprite_data.h"

#include "sprites_AI/paddle.h"
#include "sprites_AI/ball.h"
#include "sprites_AI/particle.h"

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_PADDLE] = Paddle,
    [STYPE_BALL] = Ball,
    [STYPE_PARTICLE] = Particle
};
