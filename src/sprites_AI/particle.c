#include "sprites_AI/particle.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "types.h"
#include "game_state.h"
#include "bcd.h"
#include "random.h"
#include "sprite.h"
#include "input.h"
#include "bg_clip.h"

static const u8 sParticleAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(0), 0x8A, 0
};

static const struct AnimData sParticleAnim[] = {
    [0] = {
        .oamPointer = sParticleAnim_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

#define INITIAL_SPEED -(HALF_BLOCK_SIZE)
#define GRAVITY (PIXEL_SIZE)

static const s8 sParticleFallSpeed[] = {
    INITIAL_SPEED + GRAVITY * 0,
    INITIAL_SPEED + GRAVITY * 1,
    INITIAL_SPEED + GRAVITY * 2,
    INITIAL_SPEED + GRAVITY * 3,
    INITIAL_SPEED + GRAVITY * 3,
    INITIAL_SPEED + GRAVITY * 4,
    INITIAL_SPEED + GRAVITY * 5,
    INITIAL_SPEED + GRAVITY * 6,
    INITIAL_SPEED + GRAVITY * 6,
    INITIAL_SPEED + GRAVITY * 7,
    INITIAL_SPEED + GRAVITY * 7,
    INITIAL_SPEED + GRAVITY * 8,
    INITIAL_SPEED + GRAVITY * 8,
    INITIAL_SPEED + GRAVITY * 9,
    INITIAL_SPEED + GRAVITY * 10
};

void Particle(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sParticleAnim;
        gCurrentSprite.pose = 1;
    }

    if (gFrameCounter % 4)
    {
        if (gCurrentSprite.part == PARTICLE_DIR_LEFT)
        {
            gCurrentSprite.x -= PIXEL_SIZE;
    
            if (gCurrentSprite.x > SCREEN_SIZE_X_SUB_PIXEL - BLOCK_SIZE)
                gCurrentSprite.part = PARTICLE_DIR_RIGHT;
        }
        else
        {
            gCurrentSprite.x += PIXEL_SIZE;
    
            if (gCurrentSprite.x < BLOCK_SIZE * 2)
                gCurrentSprite.part = PARTICLE_DIR_LEFT;
        }
    }

    if (gCurrentSprite.work1 < ARRAY_SIZE(sParticleFallSpeed))
    {
        gCurrentSprite.y += sParticleFallSpeed[gCurrentSprite.work1];

        if (gFrameCounter % 4 == 0)
            gCurrentSprite.work1++;
    }
    else
    {
        gCurrentSprite.y += THREE_QUARTER_BLOCK_SIZE;
    }

    if (gCurrentSprite.y > SCREEN_SIZE_Y_SUB_PIXEL)
        gCurrentSprite.status = 0;
}
