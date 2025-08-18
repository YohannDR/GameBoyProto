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
#include "random.h"
#include "io.h"
#include "fading.h"
#include "callbacks.h"
#include "game_state.h"
#include "macros.h"
#include "math.h"
#include "sprite.h"
#include "door.h"

#include "data/tilesets.h"

u8 gFrameCounter;
struct GameModeInfo gGameMode;

static void VblankCallback(void)
{
    Write8(REG_SCX, SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x));
    Write8(REG_SCY, SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y));
    Write8(REG_WX, gWindowX);
    Write8(REG_WY, gWindowY);
}

static void InitGame(void)
{
    gGameMode.main = GM_IN_GAME;

    // Enable v-blank interrupt
    Write8(REG_IE, INTR_VBLANK);

    WaitForVblank();
    Write8(REG_LCDC, 0);
    LoadGraphics(sTilesets[2]);

    SetCameraPosition(0, 0);
    PlayerInit();
    LoadRoom(0);
    SetupRandomSeed();

    CallbackSetVblank(VblankCallback);
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
                ScrollUpdate();
                // PlayerUpdate();
                DoorUpdate();
                // PlayerDraw();
                FadingUpdate();
                UpdateSpritesAsm();
            }
            else if (gGameMode.main == GM_TRANSITION)
            {
                PlayerDraw();
                TransitionUpdate();
                FadingUpdate();
                ScrollUpdate();
            }
        }

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
