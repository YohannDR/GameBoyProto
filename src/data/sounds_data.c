#include "data/sounds.h"

#include "types.h"

#include "sound.h"

const struct SoundData sSoundData[] = {
    [SOUND_UNLOCK] = {
        .channel = SOUND_CHANNEL_SQUARE_2,
        .reg1 = 0x5E,
        .reg2 = 0xA7,
        .reg3 = 0x1C,
        .reg4 = 0xC7,
    },
    [SOUND_DOOR_OPEN] = {
        .channel = SOUND_CHANNEL_SQUARE_2,
        .reg1 = 0x5E,
        .reg2 = 0xA7,
        .reg3 = 0x1C,
        .reg4 = 0x87,
    },
    [SOUND_WATER] = {
        .channel = SOUND_CHANNEL_SQUARE_4,
        .reg1 = 0x32,
        .reg2 = 0x8A,
        .reg3 = 0x14,
        .reg4 = 0xC0,
    },
    [SOUND_FIRE = {
        .channel = SOUND_CHANNEL_SQUARE_4,
        .reg1 = 0x3F,
        .reg2 = 0x87,
        .reg3 = 0x74,
        .reg4 = 0xC0,
    },
    [SOUND_PORTAL_PAST] = {
        .channel = SOUND_CHANNEL_SQUARE_1,
        .reg1 = 0x7D,
        .reg2 = 0xFF,
        .reg3 = 0x87,
        .reg4 = 0x73,
        .reg5 = 0x86
    },
    [SOUND_PORTAL_FUTUR] = {
        .channel = SOUND_CHANNEL_SQUARE_1,
        .reg1 = 0x77,
        .reg2 = 0xBF,
        .reg3 = 0x49,
        .reg4 = 0x73,
        .reg5 = 0x86
    }
};
