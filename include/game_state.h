#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "types.h"

/**
 * @brief Represents the current game mode
 * 
 */
struct GameModeInfo {
    // The main game mode
    u8 main;
    // Sub-game mode
    u8 sub;
    // Handy timer
    u8 timer;
    // Work variable
    u8 work1;
    // Work variable
    u8 work2;
};

enum GameMode {
    GM_IN_GAME,
    GM_TRANSITION
};

/**
 * @brief 8-bit frame counter
 * 
 */
extern u8 gFrameCounter;

/**
 * @brief Whether the current frame is an idle frame
 * 
 */
extern u8 gIsIdleFrame;

/**
 * @brief Game mode info
 * 
 */
extern struct GameModeInfo gGameMode;

#endif /* GAME_STATE_H */
