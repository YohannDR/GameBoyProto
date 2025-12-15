#include "data/doors.h"

const struct Door sDoors[] = {
    [0] = {
        .x = 19,
        .y = 1,
        .ownerRoom = 0,
        .height = 4,
        .width = 1,
        .targetDoor = 1,
        .exitX = -32,
        .exitY = 0,
        .tileset = 255,
    },
    [1] = {
        .x = 0,
        .y = 11,
        .ownerRoom = 1,
        .height = 4,
        .width = 1,
        .targetDoor = 0,
        .exitX = 0,
        .exitY = 0,
        .tileset = 255,
    }
};
