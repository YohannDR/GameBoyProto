#include "sprites_AI/player.h"

#include "input.h"

void Player(struct Sprite* sprite)
{
    if (gButtonInput & KEY_LEFT)
        sprite->x--;
    else if (gButtonInput & KEY_RIGHT)
        sprite->x++;

    if (gButtonInput & KEY_UP)
        sprite->y--;
    else if (gButtonInput & KEY_DOWN)
        sprite->y++;
}
