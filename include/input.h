#ifndef INPUT_H
#define INPUT_H

#include "types.h"

// Define flags for each key

#define KEY_A       (1 << 0)
#define KEY_B       (1 << 1)
#define KEY_SELECT  (1 << 2)
#define KEY_START   (1 << 3)
#define KEY_RIGHT   (1 << 4)
#define KEY_LEFT    (1 << 5)
#define KEY_UP      (1 << 6)
#define KEY_DOWN    (1 << 7)

// The pressed keys for the current frame
extern u8 gButtonInput;
// The pressed keys that differ from the previous frame
extern u8 gChangedInput;

/**
 * @brief Polls the keys for this current frame
 * 
 */
void UpdateInput(void);

#endif /* INPUT_H */
