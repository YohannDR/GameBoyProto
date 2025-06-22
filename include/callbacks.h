#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "types.h"

// Whether or not the v-blank intterupt has been fired
extern u8 gVblankFired;

void EnableInterrupts(void);
void DisableInterrupts(void);

/**
 * @brief Sets the v-blank callback
 * 
 * @param callback Callback
 */
void CallbackSetVblank(Func_T callback);

/**
 * @brief DO NOT CALL
 * 
 * Calls the v-blank callback
 * 
 */
void CallbackCallVblank(void);

/**
 * @brief Sets the lcd callback
 * 
 * @param callback Callback
 */
void CallbackSetLcd(Func_T callback);

/**
 * @brief DO NOT CALL
 * 
 * Calls the lcd callback
 * 
 */
void CallbackCallLcd(void);

/**
 * @brief Sets the timer callback
 * 
 * @param callback Callback
 */
void CallbackSetTimer(Func_T callback);

/**
 * @brief DO NOT CALL
 * 
 * Calls the timer callback
 * 
 */
void CallbackCallTimer(void);

#endif /* CALLBACKS_H */
