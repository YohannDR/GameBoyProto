#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"

/**
 * @brief Setup the random seed for software random-ness
 * 
 */
void SetupRandomSeed(void);

/**
 * @brief Generates a random number via LFSR using the Game Boy's hardware div register as a seed
 * 
 * @return u16 Random number
 */
u16 GetRandomHardware(void);

/**
 * @brief Generates a random number via LFSR using the previous random number as a seed
 * 
 * @return u16 Random number
 */
u16 GetRandomSoftware(void);

#endif /* RANDOM_H */
