#include "input.h"
#include "gb/io.h"
#include "io.h"

u8 gButtonInput;
u8 gChangedInput;

void UpdateInput(void)
{
    u8 input;
    u8 joypad;

    // Setup to read the d-pad
    Write8(REG_JOYPAD, 1 << 4);

    // Perform dummy reads
    Read8(REG_JOYPAD); Read8(REG_JOYPAD);
    joypad = Read8(REG_JOYPAD);

    // Retrieve button data
    input = joypad & 0x0F;

    // Setup to read the buttons
    Write8(REG_JOYPAD, 1 << 5);

    // Perform dummy reads
    Read8(REG_JOYPAD); Read8(REG_JOYPAD);
    joypad = Read8(REG_JOYPAD);

    // Retrieve button data
    input |= (joypad & 0x0F) << 4;

    // Flip bits, since 0 is pressed and 1 is not pressed hardware-wise, but we want the opposite
    input = ~input;

    // Update the global input variables
    gChangedInput = input & ~gButtonInput;
    gButtonInput = input;

    // Stop joypad read request
    Write8(REG_JOYPAD, 0);
}
