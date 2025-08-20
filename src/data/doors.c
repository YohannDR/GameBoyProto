#include "data/doors.h"

const struct Door sDoors[] = {
    [0] = {
        .x = 0,
        .y = 11,
        .ownerRoom = 1,
        .height = 4,
        .width = 2,
        .targetDoor = 203,
        .exitX = -32,
        .exitY = 0,
        .tileset = 255,
    },
};
