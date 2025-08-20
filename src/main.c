#include "gb/io.h"
#include "gb/oam.h"
#include "gb/display.h"
#include "gb/interrupt.h"

#include "bg.h"
#include "bg_clip.h"
#include "player.h"
#include "input.h"
#include "scroll.h"
#include "room.h"
#include "fire.h"
#include "random.h"
#include "io.h"
#include "fading.h"
#include "inventory.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"
#include "math.h"
#include "sprite.h"
#include "door.h"

#include "data/tilesets.h"

u8 gFrameCounter;
struct GameModeInfo gGameMode;

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    // Enable v-blank interrupt
    Write8(REG_IE, INTR_VBLANK);

    WaitForVblank();
    Write8(REG_LCDC, 0);
    LoadGraphics(sTilesets[1]);

    gWindowX = 0;
    gWindowY = SCREEN_SIZE_Y;
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);

    gCurrentItem = ITEM_NONE;

    SetCameraPosition(0, 0);
    PlayerInit();
    LoadRoom(0);
    SetupRandomSeed();
    InitializeWindow();

    CallbackSetVblank(VblankCallback);

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

        // If the sprite graphics loader is active, it takes absolute priority and fully hangs the game until its done
        if (gSpriteLoaderInfo.state != SPRITE_LOADER_OFF)
        {
            UpdateSpriteGraphicsLoading();
        }
        else
        {
            if (gGameMode.main == GM_IN_GAME)
            {
                // Do stuff...
                PlayerUpdate();
                DoorUpdate();
                ScrollUpdate();
            }
            else if (gGameMode.main == GM_INVENTORY)
            {
                InventoryUpdate();
            }
            else if (gGameMode.main == GM_TRANSITION)
            {
                TransitionUpdate();
                ScrollUpdate();
            }

            PlayerDraw();
            UpdateSprites();
            UpdateFire();
            FadingUpdate();
        }

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
