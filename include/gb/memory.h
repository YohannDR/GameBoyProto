#ifndef GB_MEMORY_H
#define GB_MEMORY_H

#define VRAM_BASE (void*)0x8000
#define VRAM_SIZE 0x2000

#define WRAM_BASE (void*)0xC000
#define WRAM_SIZE 0x2000

#define OAM_BASE (void*)0xFE00
#define OAM_SIZE 0x00A0

#define IO_BASE (void*)0xFF00
#define IO_SIZE 0x80

#endif /* GB_MEMORY_H */
