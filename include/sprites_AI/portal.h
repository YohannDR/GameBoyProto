#ifndef PORTAL_H
#define PORTAL_H

#include "types.h"

enum PortalType {
    PORTAL_TYPE_BIG,
    PORTAL_TYPE_SMALL,
};

extern const u8 sPortalGraphics[];

void Portal(void);

#endif /* PORTAL_H */
