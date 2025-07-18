#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

/**
 * @brief Defines values for the player's physics
 * 
 */
struct PlayerPhysics {
    // The acceleration applied when moving horizontally
    u8 xAcceleration;
    // The maximum horizontal speed
    s8 xVelocityCap;
    // The maximum vertical speed when falling
    s8 yVelocityCap;

    // Gravity value applied when the player is going upwards
    u8 gravityUpwards;
    // Gravity value applied when the player is going downwards
    u8 gravityDownwards;

    // The velocity applied to the player when jumping
    s8 jumpingVelocity;
};

/**
 * @brief Holds the current values regarding the player's movement
 * 
 */
struct PlayerMovement {
    s8 xVelocity;
    s8 yVelocity;
    u8 grounded;
};

extern struct PlayerPhysics gPlayerPhysics;
extern struct PlayerMovement gPlayerMovement;

void PlayerInit(void);

#endif /* PLAYER_H */
