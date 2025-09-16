#ifndef LOCKED_DOOR
#define LOCKED_DOOR

#include "types.h"

enum LockedDoorPose {
    LOCKED_DOOR_POSE_IDLE = 1,
    LOCKED_DOOR_POSE_UNLOCK_LOCK,
    LOCKED_DOOR_POSE_UNLOCKING,
};

extern const u8 sLockedDoorGraphics[];

void LockedDoor(void);
void DoorLock(void);

#endif /* LOCKED_DOOR */
