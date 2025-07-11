#ifndef GB_OAM_H
#define GB_OAM_H

#include "types.h"
#include "macros.h"

#define OAM_ATTR_OBP_0      (0u << 4)
#define OAM_ATTR_OBP_1      (1u << 4)
#define OAM_ATTR_X_FLIP     (1u << 5)
#define OAM_ATTR_Y_FLIP     (1u << 6)
#define OAM_ATTR_HIGH_PRIO  (0u << 7)
#define OAM_ATTR_LOW_PRIO   (1u << 7)

// Wram buffer for the oam, mapped directly to shadow oam and automatically transfered to actual oam during v-blank
extern u8 gOamBuffer[40 * 4];

/**
 * @brief Cursor in the @c gOamBuffer to indicate where the next free oam slot is
 * 
 */
extern u8 gNextOamSlot;

/**
 * @brief Gets the pointer to the oam slot in the specified slot
 *  
 * @param slot Slot
 * @return Pointer to the slot in the oam buffer
 */
#define OAM_BUFFER_SLOT(slot) (&gOamBuffer[(u8)((slot) * 4)])

/**
 * @brief Creates an oam position, takes care of sign conversion
 * 
 * @param v Value
 * @return Converted value
 */
#define OAM_POS(v) (u8)((v) < 0 ? 0x100 + (v) : (v))

/**
 * @brief Clears the oam slots that were used the previous frame
 * 
 */
void ClearAndResetOam(void);

#endif /* GB_OAM_H */
