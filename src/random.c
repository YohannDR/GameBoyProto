#include "random.h"

#include "gb/io.h"

#include "io.h"

static u16 gLfsrSeed;

static u16 LfsrFeedback(u16 input)
{
    input ^= input << 7;
    input ^= input >> 9;
    input ^= input << 8;

    return input;
}

void SetupRandomSeed(void)
{
    // The seed must NOT start at 0
    gLfsrSeed = 1;
}

u16 GetRandomHardware(void)
{
    return LfsrFeedback(Read8(REG_DIV));
}

u16 GetRandomSoftware(void)
{
    gLfsrSeed = LfsrFeedback(gLfsrSeed);
    return gLfsrSeed;
}
