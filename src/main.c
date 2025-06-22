#include "gb/io.h"
#include "gb/display.h"
#include "gb/interrupt.h"

#include "input.h"
#include "io.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"

#include "data/dungeon_map.h"
#include "data/dungeon_tiles.h"

u8 gFrameCounter;
struct GameModeInfo gGameMode;

static u8 gLcdIntrStatus;

static void TestLcdCallback(void)
{
    // Toggle status
    gLcdIntrStatus ^= 1;

    // Switch between firing the interrupt at the beginning of the screen, or in the middle
    if (gLcdIntrStatus == 0)
        Write8(REG_LYC, SCREEN_SIZE_Y / 2);
    else
        Write8(REG_LYC, 0);

    // Toggle color to split the screen in half
    Write8(REG_BGP, Read8(REG_BGP) ^ 2);
}

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    EnableInterrupts();

    // Setup LCD interrupt to fire at the middle of the screen
    Write8(REG_IE, INTR_LCD);
    Write8(REG_STAT, Read8(REG_STAT) | STAT_LCY);
    Write8(REG_LYC, SCREEN_SIZE_Y / 2);
    CallbackSetLcd(TestLcdCallback);

    // Enable display and background
    Write8(REG_LCDC, LCDC_LCD_ENABLE | LCDC_BG_ENABLE);
}

static void WaitVblank(void)
{
    // We need to check specifically for v-blank, since any other interrupt could stop this halt
    while (!gVblankFired)
    {
        // Halt the cpu while waiting for vblank
        __asm__("halt");
    }
}

void main(void)
{
    InitGame();

    for (;;)
    {
        gFrameCounter++;

        // Poll inputs immediatly
        UpdateInput();

        // Do stuff...

        // Done doing stuff, wait for v-blank
        WaitVblank();
        gVblankFired = FALSE;
    }
}
