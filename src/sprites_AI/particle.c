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

static const s8 sParticleFallSpeed[] = {
    -4, -3, -2, -1, -1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6
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
            gCurrentSprite.x -= 1;
    
            if (gCurrentSprite.x > SCREEN_SIZE_X - 7u)
                gCurrentSprite.part = PARTICLE_DIR_RIGHT;
        }
        else
        {
            gCurrentSprite.x += 1;
    
            if (gCurrentSprite.x < 16u)
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
        gCurrentSprite.y += 6;
    }

    if (gCurrentSprite.y > SCREEN_SIZE_Y)
        gCurrentSprite.status = 0;
}
