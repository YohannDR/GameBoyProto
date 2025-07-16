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

    WaitForVblank();

    Write8(REG_LCDC, 0);

    // Tile data address
    addr = (u8*)(VRAM_BASE + 0x1000);
    for (i = 0; i < ARRAY_SIZE(sLevelTileset); i++)
        *addr++ = sLevelTileset[i];

    // Load tile map
    LoadTilemap(sLevelTilemap);

    Write8(REG_BGP,  0b11100100);
    Write8(REG_OBP0, 0b11100100);
    Write8(REG_OBP1, 0b11100100);
}

static void VblankCallback(void)
{
    Write8(REG_SCX, gBackgroundInfo.x);
    Write8(REG_SCY, gBackgroundInfo.y);
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);
}

static void SetupSprites(void)
{
    SpawnSprite(SCREEN_SIZE_X_SUB_PIXEL / 2, SCREEN_SIZE_Y_SUB_PIXEL / 2, STYPE_PLAYER, 0);
}

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    // Enable v-blank intterupt
    Write8(REG_IE, INTR_VBLANK);

    LoadGraphics();
    LoadClipdata();
    SetupRandomSeed();
    SetupSprites();

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

        CheckForTilemapUpdate();
        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
