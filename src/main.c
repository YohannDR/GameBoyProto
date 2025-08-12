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
    LoadGraphics(sTilesets[0]);

    SetCameraPosition(0, 0);
    PlayerInit();
    LoadRoom(0);
    SetupRandomSeed();

    CallbackSetVblank(VblankCallback);
}

static u16 Test(u16 a)
{
    return (a & ~32) | (a % 32 + 1);
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

        if (gGameMode.main == GM_IN_GAME)
        {
            // Do stuff...
            ScrollUpdate();
            PlayerUpdate();
            DoorUpdate();
            PlayerDraw();
            FadingUpdate();
            UpdateSprites();
        }
        else if (gGameMode.main == GM_TRANSITION)
        {
            PlayerDraw();
            TransitionUpdate();
            FadingUpdate();
            ScrollUpdate();
        }

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
