#include "data/doors.h"

const struct Door sDoors[] = {
    [0] = {
        .x = 38,
        .y = 31,
        .ownerRoom = 0,
        .height = 4,
        .width = 2,
        .targetDoor = 1,
        .exitX = 0,
        .exitY = 0,
        .tileset = 255,
    },
    [1] = {
        .x = 0,
        .y = 13,
        .ownerRoom = 1,
        .height = 4,
        .width = 2,
        .targetDoor = 0,
        .exitX = 0,
        .exitY = 0,
        .tileset = 255,
    },
};
