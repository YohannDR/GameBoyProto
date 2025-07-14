#include "gb/io.h"
#include "gb/oam.h"
#include "gb/display.h"
#include "gb/interrupt.h"

#include "data/level_tilemap.h"
#include "data/level_tileset.h"

#include "bg.h"
#include "bg_clip.h"
#include "input.h"
#include "random.h"
#include "io.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"
#include "sprite.h"

u8 gFrameCounter;
struct GameModeInfo gGameMode;

static void LoadGraphics(void)
{
    u8* addr;
    u16 i;
    u8 line;
    u16 row;

    WaitForVblank();

    Write8(REG_LCDC, 0);

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

    Write8(REG_BGP,  0b11100100);
    Write8(REG_OBP0, 0b11100100);
    Write8(REG_OBP1, 0b11100100);
}

static void VblankCallback(void)
{
    Write8(REG_SCX, gBackgroundX);
    Write8(REG_SCY, gBackgroundY);
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);
}

static void SetupSprites(void)
{
    u8 paddleSlot;
    u8 ballSlot;

    paddleSlot = SpawnSprite(SCREEN_SIZE_X_SUB_PIXEL / 2, SCREEN_SIZE_Y_SUB_PIXEL - BLOCK_SIZE * 2, STYPE_PADDLE, 0);
    ballSlot = SpawnSprite(SCREEN_SIZE_X_SUB_PIXEL / 2, SCREEN_SIZE_Y_SUB_PIXEL - BLOCK_SIZE * 3, STYPE_BALL, 0);

    gSpriteData[ballSlot].work1 = paddleSlot;
}

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    // Enable v-blank intterupt
    Write8(REG_IE, INTR_VBLANK);

    LoadGraphics();
    SetupSprites();
    LoadClipdata();
    SetupRandomSeed();

    CallbackSetVblank(VblankCallback);

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
