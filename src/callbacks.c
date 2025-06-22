#include "callbacks.h"

// Whether or not the v-blank intterupt has been fired
static u8 gVblankFired;

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
    // Set this now to prevent the code below from compiling to ___sdcc_call_hl
    gVblankFired = TRUE;

    if (gVblankCallback)
        gVblankCallback();
}

void CallbackSetLcd(Func_T callback)
{
    gLcdCallback = callback;
}

void CallbackCallLcd(void)
{
    if (gLcdCallback)
        gLcdCallback();
}

void CallbackSetTimer(Func_T callback)
{
    gTimerCallback = callback;
}

void CallbackCallTimer(void)
{
    if (gTimerCallback)
        gTimerCallback();
}

void WaitForVblank(void)
{
    // We need to check specifically for v-blank, since any other interrupt could stop this halt
    while (!gVblankFired)
    {
        // Halt the cpu while waiting for vblank
        __asm__("halt");
    }

    // Reset the flag for the next v-blank
    gVblankFired = FALSE;
}
