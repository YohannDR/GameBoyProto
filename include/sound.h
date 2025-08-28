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

enum SoundChannel {
    SOUND_CHANNEL_SQUARE_1,
    SOUND_CHANNEL_SQUARE_2,
    SOUND_CHANNEL_WAVE,
    SOUND_CHANNEL_NOISE,
};

void InitSound(void);
void PlaySound(u8 sound);

#endif /* SOUND_H */
