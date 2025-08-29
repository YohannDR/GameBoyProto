#ifndef TIME_H
#define TIME_H

#include "types.h"

enum Temporality {
    TEMPORALITY_PAST,
    TEMPORALITY_FUTURE,
};

extern u8 gCurrentTemporality;

void StartPortalTransition(void);
void UpdatePortalTransition(void);

#endif /* TIME_H */
