#include "data/sprite_data.h"

#include "sprites_AI/locked_door.h"
#include "sprites_AI/portal.h"
#include "sprites_AI/pressure_plate.h"

static void Dummy(void) {}

static const u8 sDummyGraphics[] = {
    0,
};

const Func_T sSpriteAiPointers[STYPE_END] = {
    [STYPE_NONE] = Dummy,
    [STYPE_PRESSURE_PLATE] = PressurePlate,
    [STYPE_PORTAL] = Portal,
    [STYPE_LOCKED_DOOR] = LockedDoor,
    [STYPE_DOOR_LOCK] = DoorLock,
};

const u8* const sSpriteGraphicsPointers[STYPE_END] = {
    [STYPE_NONE] = sDummyGraphics,
    [STYPE_PRESSURE_PLATE] = sPressurePlateGraphics,
    [STYPE_PORTAL] = sPortalGraphics,
    [STYPE_LOCKED_DOOR] = sLockedDoorGraphics,
    [STYPE_DOOR_LOCK] = sDummyGraphics,
};
