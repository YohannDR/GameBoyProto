#include "types.h"

struct IsrVector {
    u8 opcode;
    void* func;
};

extern void LcdHandler(void);

struct IsrVector AT(0x48) __ISR_STAT = {0xc3, (void *)&(LcdHandler)};
