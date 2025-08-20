#include "data/doors.h"

const struct Door sDoors[] = {
    [0] = {
        .x = 0,
        .y = 11,
        .ownerRoom = 1,
        .height = 4,
        .width = 2,
        .targetDoor = 204,
        .exitX = -32,
        .exitY = 0,
        .tileset = 255,
    },
    [1] = {
        .x = 15,
        .y = 10,
        .ownerRoom = 2,
        .height = 4,
        .width = 3,
        .targetDoor = 0,
        .exitX = 0,
        .exitY = 0,
        .tileset = 255,
    },
};
