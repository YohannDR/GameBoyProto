#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "types.h"
#include "macros.h"

ASM_IMPL void EnableInterrupts(void);
ASM_IMPL void DisableInterrupts(void);

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

/**
 * @brief Halts the CPU and waits for the next v-blank interrupt
 * 
 */
void WaitForVblank(void);

#endif /* CALLBACKS_H */
