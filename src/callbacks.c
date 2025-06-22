#include "callbacks.h"

u8 gVblankFired;

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
