#include "data/doors.h"

const struct Door sDoors[] = {
    [0] = {
        .x = BLOCK_SIZE * 13,
        .y = BLOCK_SIZE * 14,
        .ownerRoom = 0,
        .height = 3,
        .width = 2,
        .targetDoor = 1,
        .exitX = -BLOCK_SIZE,
        .exitY = 0,
        .tileset = UCHAR_MAX
    },
    [1] = {
        .x = SCREEN_SIZE_X_SUB_PIXEL + BLOCK_SIZE * 1,
        .y = BLOCK_SIZE * 14,
        .ownerRoom = 1,
        .height = 3,
        .width = 2,
        .targetDoor = 0,
        .exitX = BLOCK_SIZE,
        .exitY = 0,
        .tileset = UCHAR_MAX
    }
};
