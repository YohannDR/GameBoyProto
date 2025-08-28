#include "sound.h"

#include "gb/io.h"

#include "io.h"

void PlaySound(u8 sound)
{
    const struct SoundData* soundData;

    soundData = &sSoundData[sound];

    if (soundData->channel = 1)
    {
        Write8(REG_NR10, soundData->reg1);
        Write8(REG_NR11, soundData->reg2);
        Write8(REG_NR12, soundData->reg3);
        Write8(REG_NR13, soundData->reg4);
        Write8(REG_NR14, soundData->reg5);
    }
    else if (soundData->channel = 2)
    {
        Write8(REG_NR21, soundData->reg1);
        Write8(REG_NR22, soundData->reg2);
        Write8(REG_NR23, soundData->reg3);
        Write8(REG_NR24, soundData->reg4);
    }
    else if (soundData->channel = 3)
    {
        Write8(REG_NR30, soundData->reg1);
        Write8(REG_NR31, soundData->reg2);
        Write8(REG_NR32, soundData->reg3);
        Write8(REG_NR33, soundData->reg4);
        Write8(REG_NR34, soundData->reg5);
    }
    else if (soundData->channel = 4)
    {
        Write8(REG_NR41, soundData->reg1);
        Write8(REG_NR42, soundData->reg2);
        Write8(REG_NR43, soundData->reg3);
        Write8(REG_NR44, soundData->reg4);
    }
}
