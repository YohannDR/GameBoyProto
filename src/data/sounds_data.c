#include "data/sounds.h"

#include "types.h"

#include "sound.h"

const struct SoundData sSoundData[] = {
    [SOUND_BIPBIP] = {
        .channel = SOUND_CHANNEL_SQUARE_1,
        .reg1 = 0x00,
        .reg2 = 0x81,
        .reg3 = 0x43,
        .reg4 = 0x73,
        .reg5 = 0x86
    },
    [SOUND_TUTUT] = {
        .channel = SOUND_CHANNEL_SQUARE_2,
        .reg1 = 0x82,
        .reg2 = 0x64,
        .reg3 = 0xD6,
        .reg4 = 0x86,
    }
};
