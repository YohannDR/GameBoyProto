#include "time.h"

#include "gb/io.h"
#include "gb/interrupt.h"
#include "gb/display.h"

#include "callbacks.h"
#include "game_state.h"
#include "io.h"

u8 gCurrentTemporality;

static u8 gBgPaletteBackup;
static u8 gObj0PaletteBackup;
static u8 gObj1PaletteBackup;
static u8 gTransitionDirection;

enum PortalState {
    PORTAL_STATE_HIDING_SCREEN,
    PORTAL_STATE_TRANSITION,
    PORTAL_STATE_REVEALING_SCREEN,
};

#define TIME_TRANSITION_SPEED (3u)

/*static*/ void PortalHidingDownVblank(void)
{
    Write8(REG_BGP, PALETTE_BLACK);
    Write8(REG_OBP0, PALETTE_BLACK);
    Write8(REG_OBP1, PALETTE_BLACK);

    // Setup to break on the next line on the next frame
    if (gTransitionDirection)
        gGameMode.work1 += TIME_TRANSITION_SPEED;
    else
        gGameMode.work1 -= TIME_TRANSITION_SPEED;

    Write8(REG_LYC, gGameMode.work1);
}

/*static*/ void PortalHidingDownHblank(void)
{
    if (Read8(REG_LY) == 0 && !gTransitionDirection)
    {
        // Hacky fix for the single frame flicker due to h-blank being triggered during the first scanline of v-blank (I think?)
        return;
    }

    Write8(REG_BGP, gBgPaletteBackup);
    Write8(REG_OBP0, gObj0PaletteBackup);
    Write8(REG_OBP1, gObj1PaletteBackup);
}

/*static*/ void PortalHidingUpVblank(void)
{
    Write8(REG_BGP, gBgPaletteBackup);
    Write8(REG_OBP0, gObj0PaletteBackup);
    Write8(REG_OBP1, gObj1PaletteBackup);

    // Setup to break on the next line on the next frame
    if (gTransitionDirection)
        gGameMode.work1 -= TIME_TRANSITION_SPEED;
    else
        gGameMode.work1 += TIME_TRANSITION_SPEED;

    Write8(REG_LYC, gGameMode.work1);
}

/*static*/ void PortalHidingUpHblank(void)
{
    Write8(REG_BGP, PALETTE_BLACK);
    Write8(REG_OBP0, PALETTE_BLACK);
    Write8(REG_OBP1, PALETTE_BLACK);
}

static void ChangeTime(u8 newTime)
{
    // Update fire
    // Update room specific stuff
    gCurrentTemporality = newTime;
}

void StartPortalTransition(void)
{
    gGameMode.main = GM_PORTAL;
    gGameMode.sub = PORTAL_STATE_HIDING_SCREEN;

    gBgPaletteBackup = Read8(REG_BGP);
    gObj0PaletteBackup = Read8(REG_OBP0);
    gObj1PaletteBackup = Read8(REG_OBP1);

    WaitForVblank();

    gTransitionDirection = TRUE;

    if (gCurrentTemporality == TEMPORALITY_PAST)
    {
        CallbackSetVblank(PortalHidingDownVblank);
        CallbackSetLcd(PortalHidingDownHblank);

        gGameMode.work1 = 0;
    }
    else
    {
        CallbackSetVblank(PortalHidingUpVblank);
        CallbackSetLcd(PortalHidingUpHblank);

        gGameMode.work1 = SCREEN_SIZE_Y;
    }

    // Enable h-blank interrupt
    Write8(REG_LYC, gGameMode.work1);
    Write8(REG_STAT, Read8(REG_STAT) | STAT_LCY);
    Write8(REG_IE, Read8(REG_IE) | INTR_LCD);
}

void UpdatePortalTransition(void)
{
    if (gGameMode.sub == PORTAL_STATE_HIDING_SCREEN)
    {
        if ((gCurrentTemporality == TEMPORALITY_PAST && gGameMode.work1 == SCREEN_SIZE_Y) ||
            (gCurrentTemporality == TEMPORALITY_FUTURE && gGameMode.work1 == 0))
        {
            // Disable intterupts
            Write8(REG_IE, Read8(REG_IE) & ~INTR_LCD);

            Write8(REG_BGP, PALETTE_BLACK);
            Write8(REG_OBP0, PALETTE_BLACK);
            Write8(REG_OBP1, PALETTE_BLACK);

            gGameMode.sub = PORTAL_STATE_TRANSITION;
        }
    }
    else if (gGameMode.sub == PORTAL_STATE_TRANSITION)
    {
        gGameMode.sub = PORTAL_STATE_REVEALING_SCREEN;

        WaitForVblank();

        gTransitionDirection = FALSE;
        if (gCurrentTemporality == TEMPORALITY_PAST)
        {
            CallbackSetVblank(PortalHidingDownVblank);
            CallbackSetLcd(PortalHidingDownHblank);

            gGameMode.work1 = SCREEN_SIZE_Y - TIME_TRANSITION_SPEED;
        }
        else
        {
            CallbackSetVblank(PortalHidingUpVblank);
            CallbackSetLcd(PortalHidingUpHblank);

            gGameMode.work1 = 0;
        }

        // Enable h-blank interrupt
        Write8(REG_LYC, gGameMode.work1);
        Write8(REG_IE, Read8(REG_IE) | INTR_LCD);
    }
    else if (gGameMode.sub == PORTAL_STATE_REVEALING_SCREEN)
    {
        if ((gCurrentTemporality == TEMPORALITY_PAST && gGameMode.work1 == 0) ||
            (gCurrentTemporality == TEMPORALITY_FUTURE && gGameMode.work1 == SCREEN_SIZE_Y))
        {
            // Disable h-blank interrupt
            Write8(REG_IE, Read8(REG_IE) & ~INTR_LCD);
            Write8(REG_STAT, Read8(REG_STAT) & ~STAT_LCY);

            Write8(REG_BGP, gBgPaletteBackup);
            Write8(REG_OBP0, gObj0PaletteBackup);
            Write8(REG_OBP1, gObj1PaletteBackup);

            gGameMode.main = GM_IN_GAME;

            CallbackSetVblank(VblankCallback);
        }
    }
}
