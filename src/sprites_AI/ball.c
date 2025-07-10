#include "sprites_AI/ball.h"

#include "gb/display.h"

#include "types.h"
#include "bcd.h"
#include "random.h"
#include "sprite.h"
#include "input.h"
#include "bg_clip.h"

static const u8 sBallAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    0, 0, 0x8F, 0
};

static const struct AnimData sBallAnim[] = {
    [0] = {
        .oamPointer = sBallAnim_Frame0,
        .duration = UCHAR_MAX
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

enum BallPose {
    WAITING_TO_START = 1,
    MOVING
};

static void DestroyBrick(u8 x, u8 y, u8 leftPart, u8 flag)
{
    u8 otherX;

    if (leftPart)
        otherX = x + 8;
    else
        otherX = x - 8;

    x = OBJ_TO_TILE_X(x);
    y = OBJ_TO_TILE_Y(y);
    otherX = OBJ_TO_TILE_X(otherX);

    SetBgValue(x, y, 0x80);
    SetBgValue(otherX, y, 0x80);

    SetClipdataValue(x, y, CLIPDATA_AIR);
    SetClipdataValue(otherX, y, CLIPDATA_AIR);

    gCurrentSprite.status ^= flag;

    gCurrentSprite.work3 = BcdAdd(gCurrentSprite.work3, 1);
    BcdDraw(5, 17, gCurrentSprite.work3);
}

static void BallCheckBrickOnSide(s8 offset)
{
    u8 block;
    u8 xPosition;

    xPosition = gCurrentSprite.x + offset;

    block = GetClipdataValue(OBJ_TO_TILE_X(xPosition), OBJ_TO_TILE_Y(gCurrentSprite.y));

    if (block == CLIPDATA_LEFT_BRICK)
    {
        DestroyBrick(xPosition, gCurrentSprite.y, TRUE, SPRITE_STATUS_MOVING_RIGHT);
    }
    else if (block == CLIPDATA_RIGHT_BRICK)
    {
        DestroyBrick(xPosition, gCurrentSprite.y, FALSE, SPRITE_STATUS_MOVING_RIGHT);
    }
}

static void BallCheckBrickTopDown(s8 offset)
{
    u8 block;
    u8 yPosition;

    yPosition = gCurrentSprite.y + offset;

    block = GetClipdataValue(OBJ_TO_TILE_X(gCurrentSprite.x), OBJ_TO_TILE_Y(yPosition));

    if (block == CLIPDATA_LEFT_BRICK)
    {
        DestroyBrick(gCurrentSprite.x, yPosition, TRUE, SPRITE_STATUS_MOVING_DOWN);
    }
    else if (block == CLIPDATA_RIGHT_BRICK)
    {
        DestroyBrick(gCurrentSprite.x, yPosition, FALSE, SPRITE_STATUS_MOVING_DOWN);
    }
}

static void BallRespawn(void)
{
    struct Sprite* paddle;

    // Put at spawn point
    gCurrentSprite.x = SCREEN_SIZE_X / 2;
    gCurrentSprite.y = SCREEN_SIZE_Y - 8u;

    // Clear moving direction
    gCurrentSprite.status &= ~(SPRITE_STATUS_MOVING_RIGHT | SPRITE_STATUS_MOVING_DOWN);

    gCurrentSprite.pose = WAITING_TO_START;
    gCurrentSprite.work2--;

    DrawNumber(2, 17, gCurrentSprite.work2);

    paddle = &gSpriteData[gCurrentSprite.work1];
    paddle->pose = WAITING_TO_START;
    paddle->x = gCurrentSprite.x;
    paddle->y = gCurrentSprite.y + 8u;
}

static void BallPaddleCollision(void)
{
    const struct Sprite* paddle;

    paddle = &gSpriteData[gCurrentSprite.work1];

    if (gCurrentSprite.y < paddle->y - 4)
        return;

    if (gCurrentSprite.x > paddle->x - 15u && gCurrentSprite.x < paddle->x + 15u)
        gCurrentSprite.status &= ~SPRITE_STATUS_MOVING_DOWN;
}

void Ball(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sBallAnim;
        // Total amount of balls
        gCurrentSprite.work2 = 5;
        // Score
        gCurrentSprite.work3 = 0;
        gCurrentSprite.pose = WAITING_TO_START;

        DrawNumber(2, 17, gCurrentSprite.work2);
        DrawNumber(5, 17, 0);
    }

    if (gCurrentSprite.pose == WAITING_TO_START)
    {
        if (gChangedInput & KEY_A)
        {
            gCurrentSprite.pose = MOVING;
            if (GetRandomHardware() % 2)
                gCurrentSprite.status |= SPRITE_STATUS_MOVING_RIGHT;
        }

        return;
    }

    if (gCurrentSprite.status & SPRITE_STATUS_MOVING_RIGHT)
    {
        gCurrentSprite.x++;

        if (gCurrentSprite.x > SCREEN_SIZE_X - 7u)
            gCurrentSprite.status &= ~SPRITE_STATUS_MOVING_RIGHT;

        BallCheckBrickOnSide(4);
    }
    else
    {
        gCurrentSprite.x--;

        if (gCurrentSprite.x < 16u)
            gCurrentSprite.status |= SPRITE_STATUS_MOVING_RIGHT;

        BallCheckBrickOnSide(-4);
    }

    if (gCurrentSprite.status & SPRITE_STATUS_MOVING_DOWN)
    {
        gCurrentSprite.y++;
        BallPaddleCollision();

        if (gCurrentSprite.y > SCREEN_SIZE_Y + 16u)
        {
            BallRespawn();
            return;
        }

        BallCheckBrickTopDown(4);
    }
    else
    {
        gCurrentSprite.y--;

        if (gCurrentSprite.y < 24u)
            gCurrentSprite.status |= SPRITE_STATUS_MOVING_DOWN;

        BallCheckBrickTopDown(-4);
    }
}
