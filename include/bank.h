#ifndef BANK_H
#define BANK_H

#include "types.h"

/**
 * @brief The different banks and their contents
 * 
 */
enum Bank {
    // Common bank (0)
    BANK_COMMON,
    // Data bank (tilesets, rooms, doors)
    BANK_DATA,
    // Sprite bank (AIs, data)
    BANK_SPRITE
};

extern u8 gCurrentBank;
extern u8 gPreviousBank;

/**
 * @brief Initializes the bank state, defaults to bank 1 (data)
 * 
 */
void BankInit(void);

/**
 * @brief Changes the currently selected bank
 * 
 * @param bank Bank number
 */
void SwitchBank(u8 bank);

/**
 * @brief Selects the previous bank
 * 
 */
void BankGoBack(void);

#endif /* BANK_H */
