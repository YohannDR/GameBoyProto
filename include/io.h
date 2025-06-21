#ifndef IO_H
#define IO_H

#include "types.h"

#define Write64(addr, val) (*(vu64 *)(addr)) = (val)
#define Write32(addr, val) (*(vu32 *)(addr)) = (val)
#define Write16(addr, val) (*(vu16 *)(addr)) = (val)
#define Write8(addr, val) (*(vu8 *)(addr)) = (val)

#define Read64(addr) (*(vu64 *)(addr))
#define Read32(addr) (*(vu32 *)(addr))
#define Read16(addr) (*(vu16 *)(addr))
#define Read8(addr) (*(vu8 *)(addr))

#endif /* IO_H */
