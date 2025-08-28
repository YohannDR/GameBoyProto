#ifndef SOUND_H
#define SOUND_H

#include "types.h"

/**
 * @brief Represents a sound effect
 * 
 */
struct SoundData {
    u8 channel;
    u8 reg1;
    u8 reg2;
    u8 reg3;
    u8 reg4;
    u8 reg5;
};

enum Sound {
    SOUND_BIPBIP,
    SOUND_TUTUT,
};

/**
 * Think about to turn the sound ON during the game init with this 3 lines.
  NR52_REG = 0x80;
  NR50_REG = 0x77;
  NR51_REG = 0xFF;
 */

void PlaySound(u8 sound);

#endif /* SOUND_H */
