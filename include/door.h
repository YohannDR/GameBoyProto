#ifndef DOOR_H
#define DOOR_H

#include "types.h"
#include "macros.h"

/**
 * @brief Represents a door, an object that can trigger transitions between different rooms
 * 
 */
struct Door {
    // X position, in sub pixels
    u16 x;
    // Y position, in sub pixels
    u16 y;
    // In which room this door is
    u8 ownerRoom;
    // Height, in blocks
    u8 height;
    // Width, in blocks
    u8 width;
    // Target door
    u8 targetDoor;
    // Exit distance X offset
    s8 exitX;
    // Exit distance Y offset
    s8 exitY;
    // The tileset that this door should load
    u8 tileset;
};

struct DoorTransition {
    u8 type;
    u8 stage;
    u8 direction;
    u8 timer;
};

/**
 * @brief The type of room transition
 * 
 */
enum TransitionType {
    // No transition currently active
    TRANSITION_TYPE_NONE,
    // A normal transition, nothing is loaded, no fade, just a simple scroll
    TRANSITION_TYPE_NORMAL,
    // A transition that requires loading graphics, has fading
    TRANSITION_TYPE_LOADING,
};

enum TransitionStageNormal {
    TRANSITION_STAGE_NORMAL_NONE,
    TRANSITION_STAGE_NORMAL_SCROLLING,
};

// Currently loaded doors
extern struct Door gDoors[4];
extern struct DoorTransition gDoorTransition;

// "Invalid" door id
#define DOOR_NONE UCHAR_MAX

/**
 * @brief Resets the doors
 * 
 */
void DoorReset(void);

/**
 * @brief Loads a door into memory
 * 
 * @param door Door
 */
void DoorLoad(const struct Door* door);

/**
 * @brief Updates the doors, checks for collision with the player
 * 
 */
void DoorUpdate(void);

void TransitionUpdate(void);

ASM_IMPL void UpdateTransitionVblank(void);

#endif /* DOOR_H */
