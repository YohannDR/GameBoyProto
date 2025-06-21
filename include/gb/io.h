#ifndef GB_IO_H
#define GB_IO_H

#include "gb/memory.h"

#define REG_JOYPAD (IO_BASE + 0x00)

#define REG_SB (IO_BASE + 0x01)
#define REG_SC (IO_BASE + 0x02)

#define REG_DIV (IO_BASE + 0x04)
#define REG_TIMA (IO_BASE + 0x05)
#define REG_TAC (IO_BASE + 0x07)

#define REG_NR10 (IO_BASE + 0x10)
#define REG_NR11 (IO_BASE + 0x11)
#define REG_NR12 (IO_BASE + 0x12)
#define REG_NR13 (IO_BASE + 0x13)
#define REG_NR14 (IO_BASE + 0x14)
#define REG_NR21 (IO_BASE + 0x16)
#define REG_NR22 (IO_BASE + 0x17)
#define REG_NR23 (IO_BASE + 0x18)
#define REG_NR24 (IO_BASE + 0x19)
#define REG_NR30 (IO_BASE + 0x1A)
#define REG_NR31 (IO_BASE + 0x1B)
#define REG_NR32 (IO_BASE + 0x1C)
#define REG_NR33 (IO_BASE + 0x1D)
#define REG_NR34 (IO_BASE + 0x1E)
#define REG_NR41 (IO_BASE + 0x20)
#define REG_NR42 (IO_BASE + 0x21)
#define REG_NR43 (IO_BASE + 0x22)
#define REG_NR44 (IO_BASE + 0x23)
#define REG_NR50 (IO_BASE + 0x24)
#define REG_NR51 (IO_BASE + 0x25)
#define REG_NR52 (IO_BASE + 0x26)

#define REG_IF (IO_BASE + 0x0F)

#define REG_LCDC (IO_BASE + 0x40)
#define REG_STAT (IO_BASE + 0x41)
#define REG_SCY (IO_BASE + 0x42)
#define REG_SCX (IO_BASE + 0x43)
#define REG_LY (IO_BASE + 0x44)
#define REG_LYC (IO_BASE + 0x45)
#define REG_DMA (IO_BASE + 0x46)
#define REG_BGP (IO_BASE + 0x47)
#define REG_OBP0 (IO_BASE + 0x48)
#define REG_OBP1 (IO_BASE + 0x49)
#define REG_WY (IO_BASE + 0x4A)
#define REG_WX (IO_BASE + 0x4B)

#define REG_IF (IO_BASE + 0x0F)
#define REG_IE (IO_BASE + 0xFF)

#endif /* GB_IO_H */
