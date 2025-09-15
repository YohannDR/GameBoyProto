#include "callbacks.h"

#include "gb/io.h"

#include "bg.h"
#include "io.h"
#include "game_state.h"

u8 gIgnoreIdleFrame;

// Whether or not the v-blank intterupt has been fired
vu8 gVblankFired;

// Function pointers for interrupt callbacks

static Func_T gVblankCallback;
static Func_T gLcdCallback;
static Func_T gTimerCallback;

void CallbackSetVblank(Func_T callback)
{
    gVblankCallback = callback;
}

void CallbackCallVblank(void)
{
    Func_T func;

    // Use a local variable to avoid reading the value of the global variable twice, same for every other callback below
    func = gVblankCallback;
    if (func)
        func();
}

void CallbackSetLcd(Func_T callback)
{
    gLcdCallback = callback;
}

void CallbackCallLcd(void)
{
    Func_T func;

    func = gLcdCallback;
    if (func)
        func();
}

void CallbackCallTimer(void)
{
    Func_T func;

    func = gTimerCallback;
    if (func)
        func();
}

void WaitForVblank(void)
{
    gVblankFired = FALSE;

    // We need to check specifically for v-blank, since any other interrupt could stop this halt
    while (!gVblankFired)
    {
        // Halt the cpu while waiting for vblank
        ASM("halt");
        ASM("nop");
    }

    // Reset the flag for the next v-blank
    gVblankFired = FALSE;
}

void VblankCallback(void)
{
    Write8(REG_SCX, gBackgroundX);
    Write8(REG_SCY, gBackgroundY);
}
