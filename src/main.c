#include "gb/io.h"
#include "gb/oam.h"
#include "gb/display.h"
#include "gb/interrupt.h"

#include "data/level_tilemap.h"
#include "data/level_tileset.h"

#include "input.h"
#include "io.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"
#include "sprite.h"

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
    Write8(REG_OBP0, Read8(REG_OBP0) ^ (1 << 3));
    Write8(REG_OBP1, Read8(REG_OBP1) ^ (1 << 3));
}

static void LoadGraphics(void)
{
    WaitForVblank();

    Write8(REG_LCDC, 0);

    u8* addr;
    u16 i;
    u8 line;
    u16 row;

    // Tile data address
    addr = (u8*)(VRAM_BASE + 0x0800);
    for (i = 0; i < ARRAY_SIZE(sLevelTileset); i++)
        *addr++ = sLevelTileset[i];

    // Load tile map
    addr = (u8*)(VRAM_BASE + 0x1800);
    line = 0;
    row = 0;

    for (i = 0; i < ARRAY_SIZE(sLevelTilemap); i++)
    {
        *addr++ = sLevelTilemap[i];
        row++;

        // Properly wrap around horizontally
        if (row == LEVEL01_TILEMAP_WIDTH)
        {
            row = 0;
            addr += 32 - LEVEL01_TILEMAP_WIDTH;
        }
    }

    Write8(REG_BGP, 0b11100100);
    Write8(REG_OBP0, 0b11100100);
    Write8(REG_OBP1, 0b11100100);
}

static void SetupSprites(void)
{
    SpawnSprite(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y, STYPE_PADDLE);
    SpawnSprite(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y - 8u, STYPE_BALL);
}

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    // Enable v-blank intterupt
    Write8(REG_IE, INTR_VBLANK);

    LoadGraphics();
    SetupSprites();
    
    // Enable display, background and objects
    Write8(REG_LCDC, LCDC_LCD_ENABLE | LCDC_BG_ENABLE | LCDC_OBJ_ENABLE);
}

void main(void)
{
    InitGame();

    for (;;)
    {
        gFrameCounter++;

        // Poll inputs immediatly
        UpdateInput();

        ClearAndResetOam();

        // Do stuff...
        UpdateSprites();

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
