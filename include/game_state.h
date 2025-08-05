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
    // Which game mode should be next
    u8 next;
};

enum GameMode {
    GM_IN_GAME,
    GM_TRANSITION
};

// 8-bit frame counter
extern u8 gFrameCounter;

// Game mode info
extern struct GameModeInfo gGameMode;

#endif /* GAME_STATE_H */
