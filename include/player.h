#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

#define PLAYER_HEIGHT (BLOCK_SIZE * 3)
#define PLAYER_WIDTH  (BLOCK_SIZE * 2)

#define JUMP_BUTTON (KEY_A)
#define INTERACTION_BUTTON (KEY_B)

struct PlayerData {
    u16 x;
    u16 y;

    u8 pose;

    // Current animation frame
    u8 currentAnimFrame;
    // Timer for the current animation frame
    u8 animTimer;
    // Pointer to the current animation
    const struct AnimData* animPointer;

    u8 properties;
    u8 size;

    // Work variable, essentially free space
    u8 work1;
    // Work variable, essentially free space
    u8 work2;
    // Work variable, essentially free space
    u8 work3;
};

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
    u8 direction;
};

enum PlayerPose {
    PLAYER_POSE_IDLE,
    PLAYER_POSE_RUNNING,
    PLAYER_POSE_JUMPING,
    PLAYER_POSE_ON_LADDER
};

enum PlayerSize {
    PLAYER_SIZE_BIG,
    PLAYER_SIZE_SMALL
};

extern struct PlayerData gPlayerData;
extern struct PlayerPhysics gPlayerPhysics;
extern struct PlayerMovement gPlayerMovement;

void PlayerInit(void);
void PlayerSetPose(u8 pose);
void PlayerUpdate(void);
void PlayerDraw(void);

#endif /* PLAYER_H */
