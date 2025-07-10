#ifndef BCD_H
#define BCD_H

#include "types.h"

/**
 * @file Provides utilities to handle binary coded decimal (BCD) numbers
 * 
 * A BCD number is a number, represented in binary, that can easily be converted to decimal
 * 
 * This works by having each nibble of a byte represent a digit of the decimal number, the lower nibble is the ones
 * and the upper nibble is the tens.
 * 
 * For example, the decimal number '13' is normally representted in binary as such :
 * `0b0000_1101`
 * 
 * However, in BCD, it would be like this :
 * `0b0001_0011`
 * The upper nibble represents the tens, so the '1' in this case,
 * and the lower nibble represents the ones, the '3' in this case
 * 
 * Using BCD is useful when the number needs to have its digits separated often, such as when it needs to be drawn.
 */

/**
 * @brief Type for a binary coded decimal number
 * 
 */
typedef u8 Bcd;

/**
 * @brief Creates a binary coded decimal number from a normal number
 * 
 * @param number Normal number
 * @return Bcd Binary coded decimal representation
 */
Bcd BcdCreate(u8 number);

/**
 * @brief Adds a value to a binary coded decimal number
 * 
 * @param number Binary coded decimal number
 * @param add Value to add
 * @return Bcd Result
 */
Bcd BcdAdd(Bcd number, u8 add);

/**
 * @brief Draws a binary coded decimal number
 * 
 * @param x X position
 * @param y Y position
 * @param number Binary coded decimal number
 */
void BcdDraw(u8 x, u8 y, Bcd number);

#endif /* BCD_H */
