#ifndef MATH_H
#define MATH_H

#include "types.h"
#include "macros.h"

/**
 * @file Groups some math functions that are ultra optimized in assembly, because the compiler goes ballistic sometimes
 * 
 * The definitions of the functions can be found in asm/math.s
 */

/**
 * @brief Computes an array index using sprite coordinates :
 * 
 * (y / 32 - 2) * width + x / 32 - 1
 * 
 * @param y Y position
 * @param width Array width
 * @param x X position
 * @return u8 Index
 */
ASM_IMPL u16 ComputeIndexFromSpriteCoords(u16 y, u8 width, u16 x);

#endif /* MATH_H */
