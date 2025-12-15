#ifndef LOCKED_DOOR_H
#define LOCKED_DOOR_H

#include "types.h"

enum LockedDoorPose {
    LOCKED_DOOR_POSE_IDLE = 1,
    LOCKED_DOOR_POSE_UNLOCK_LOCK,
};

extern const u8 sLockedDoorGraphics[];

void LockedDoor(void);
void DoorLock(void);

#endif /* LOCKED_DOOR_H */
