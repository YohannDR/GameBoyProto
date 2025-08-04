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
    Write8(REG_SCX, gBackgroundInfo.x);
    Write8(REG_SCY, gBackgroundInfo.y);
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
    LoadRoom(0);
    SetupRandomSeed();
    PlayerInit();

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

        // Do stuff...
        UpdateSprites();
        PlayerUpdate();
        // DoorUpdate();

        PlayerDraw();
        FadingUpdate();
        ScrollUpdate();

        // Done doing stuff, wait for v-blank
        WaitForVblank();
    }
}
