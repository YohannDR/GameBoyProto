#ifndef FADING_H
#define FADING_H

#include "types.h"

/**
 * @brief Holds data about the color fading
 * 
 */
struct FadingInfo {
    // Which palettes the fading target
    u8 target;
    // Timer
    u8 timer;
    // The interval on which the fading is update
    u8 interval;
    // The palette targets for each fading target
    u8 targetPalettes[3];
};

/**
 * @brief Fading palette targets
 * 
 */
enum FadingTarget {
    // No target
    FADING_TARGET_NONE          = 0,
    // Background palette
    FADING_TARGET_BACKGROUND    = 1 << 0,
    // Object 0 palette
    FADING_TARGET_OBJ0          = 1 << 1,
    // Object 1 palette
    FADING_TARGET_OBJ1          = 1 << 2,
};

extern u8 gBackgroundPalette;
extern u8 gObj0Palette;
extern u8 gObj1Palette;
extern struct FadingInfo gColorFading;

/**
 * @brief Starts a palette fading
 * 
 * @param target Palette to target
 * @param color Target color
 */
void FadingStart(u8 target, u8 color, u8 interval);

/**
 * @brief Updates the palette fading
 * 
 */
void FadingUpdate(void);

#endif /* FADING_H */
