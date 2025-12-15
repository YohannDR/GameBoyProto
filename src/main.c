#include "gb/io.h"
#include "gb/oam.h"
#include "gb/display.h"
#include "gb/interrupt.h"

#include "bg.h"
#include "bg_clip.h"
#include "bank.h"
#include "player.h"
#include "input.h"
#include "scroll.h"
#include "room.h"
#include "io.h"
#include "fading.h"
#include "gfx_loader.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"
#include "math.h"
#include "sprite.h"
#include "door.h"
#include "sound.h"

#include "data/tilesets.h"

u8 gFrameCounter;
struct GameModeInfo gGameMode;

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    BankInit();

    // Enable v-blank interrupt
    Write8(REG_IE, INTR_VBLANK);

    WaitForVblank();
    Write8(REG_LCDC, 0);
    LoadGraphics(sTilesets[0]);

    gWindowX = 0;
    gWindowY = SCREEN_SIZE_Y;
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);

    InitSound();

    SetCameraPosition(0, 0);
    PlayerInit();
    LoadRoom(0);

    CallbackSetVblank(VblankCallback);

    gObj0Palette = PALETTE_ALL;
    gObj1Palette = PALETTE_ALL;

    Write8(REG_OBP0, gObj0Palette);
    Write8(REG_OBP1, gObj1Palette);

    // Enable display, background and objects
    Write8(REG_LCDC, LCDC_LCD_ENABLE | LCDC_BG_ENABLE | LCDC_OBJ_ENABLE | LCDC_WINDOW_ENABLE | LCDC_WINDOW_TILEMAP_SELECT);
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

        // If the graphics loader is active, it takes absolute priority and fully hangs the game until its done
        if (gGraphicsLoaderInfo.state != GRAPHICS_LOADER_OFF)
        {
            if (gGraphicsLoaderInfo.state & GRAPHICS_LOADER_TILESET)
                TransitionUpdate();
            else
                UpdateSpriteGraphicsLoading();
        }
        else
        {
            if (gGameMode.main == GM_IN_GAME)
            {
                PlayerUpdate();
                DoorUpdate();
                ScrollUpdate();
            }
            else if (gGameMode.main == GM_TRANSITION)
            {
                TransitionUpdate();
            }

            PlayerDraw();
            UpdateSprites();
            FadingUpdate();
        }

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
