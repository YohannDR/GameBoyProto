#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "types.h"
#include "macros.h"

/**
 * @brief Calls the callbacks every "frame", regardless of the idle frame status
 * (i.e. enables 60fps callbacks even when running at 30fps)
 * 
 */
extern u8 gIgnoreIdleFrame;

/**
 * @brief Sets the v-blank callback
 * 
 * @param callback Callback
 */
void CallbackSetVblank(Func_T callback);

/**
 * @brief Sets the lcd callback
 * 
 * @param callback Callback
 */
void CallbackSetLcd(Func_T callback);

/**
 * @brief Halts the CPU and waits for the next v-blank interrupt
 * 
 */
void WaitForVblank(void);

/**
 * @brief Generic v-blank callback
 * 
 */
void VblankCallback(void);

#endif /* CALLBACKS_H */
