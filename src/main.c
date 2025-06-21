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

static void InitGame(void)
{
    gFrameCounter = 0;

    gButtonInput = 0;
    gChangedInput = 0;

    gGameMode.main = GM_IN_GAME;
    gGameMode.sub = 0;
    gGameMode.timer = 0;
    gGameMode.next = 0;

    EnableInterrupts();

    Write8(REG_IE, INTR_LCD);
    Write8(REG_STAT, Read8(REG_STAT) | STAT_LCY);

    Write8(REG_LYC, SCREEN_SIZE_Y / 2);

    // Enable display and background
    Write8(REG_LCDC, LCDC_LCD_ENABLE | LCDC_BG_ENABLE);
}

static void WaitVblank(void)
{
    // Wait for the LY register to reach the end of the rendered screen, just before v-blank
    while (Read8(REG_LY) != SCREEN_SIZE_Y) {}
}

void main(void)
{
    InitGame();

    for (;;)
    {
        UpdateInput();

        gFrameCounter++;

        switch (gGameMode.main)
        {
            case GM_IN_GAME:
        }

        // WaitVblank();
    }
}
