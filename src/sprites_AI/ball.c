#include "sprites_AI/ball.h"

#include "gb/oam.h"
#include "gb/display.h"

#include "types.h"
#include "bcd.h"
#include "random.h"
#include "sprite.h"
#include "input.h"
#include "bg_clip.h"

#include "sprites_AI/particle.h"

static const u8 sBallAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(0), OAM_POS(0), 0x8F, 0
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

#define PADDLE_HITBOX_SIZE (BLOCK_SIZE + THREE_QUARTER_BLOCK_SIZE)

static void DestroyBrick(u16 x, u16 y, u8 leftPart, u8 flag)
{
    u16 otherX;

    if (leftPart)
        otherX = x + BLOCK_SIZE;
    else
        otherX = x - BLOCK_SIZE;

    // Spawn the particles first, before the positions are converted
    SpawnSprite(otherX, y, STYPE_PARTICLE, PARTICLE_DIR_LEFT);
    SpawnSprite(otherX, y, STYPE_PARTICLE, PARTICLE_DIR_RIGHT);

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
    u16 xPosition;

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
    u16 yPosition;

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
    gCurrentSprite.x = SCREEN_SIZE_X_SUB_PIXEL / 2;
    gCurrentSprite.y = SCREEN_SIZE_Y_SUB_PIXEL - BLOCK_SIZE * 3;

    // Clear moving direction
    gCurrentSprite.status &= ~(SPRITE_STATUS_MOVING_RIGHT | SPRITE_STATUS_MOVING_DOWN);

    gCurrentSprite.pose = WAITING_TO_START;
    gCurrentSprite.work2--;

    DrawNumber(2, 17, gCurrentSprite.work2);

    paddle = &gSpriteData[gCurrentSprite.work1];
    paddle->pose = WAITING_TO_START;
    paddle->x = gCurrentSprite.x;
    paddle->y = gCurrentSprite.y + BLOCK_SIZE;
}

static void BallPaddleCollision(void)
{
    const struct Sprite* paddle;

    paddle = &gSpriteData[gCurrentSprite.work1];

    if (gCurrentSprite.y < paddle->y - HALF_BLOCK_SIZE)
        return;

    if (gCurrentSprite.x > paddle->x - PADDLE_HITBOX_SIZE && gCurrentSprite.x < paddle->x + PADDLE_HITBOX_SIZE)
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
        gCurrentSprite.x += PIXEL_SIZE;

        if (gCurrentSprite.x > SCREEN_SIZE_X_SUB_PIXEL - BLOCK_SIZE)
            gCurrentSprite.status &= ~SPRITE_STATUS_MOVING_RIGHT;

        BallCheckBrickOnSide(HALF_BLOCK_SIZE);
    }
    else
    {
        gCurrentSprite.x -= PIXEL_SIZE;

        if (gCurrentSprite.x < BLOCK_SIZE * 2)
            gCurrentSprite.status |= SPRITE_STATUS_MOVING_RIGHT;

        BallCheckBrickOnSide(-HALF_BLOCK_SIZE);
    }

    if (gCurrentSprite.status & SPRITE_STATUS_MOVING_DOWN)
    {
        gCurrentSprite.y += PIXEL_SIZE;
        BallPaddleCollision();

        if (gCurrentSprite.y > SCREEN_SIZE_Y_SUB_PIXEL - BLOCK_SIZE * 2)
        {
            BallRespawn();
            return;
        }

        BallCheckBrickTopDown(HALF_BLOCK_SIZE);
    }
    else
    {
        gCurrentSprite.y -= PIXEL_SIZE;

        if (gCurrentSprite.y < BLOCK_SIZE * 3)
            gCurrentSprite.status |= SPRITE_STATUS_MOVING_DOWN;

        BallCheckBrickTopDown(-HALF_BLOCK_SIZE);
    }
}
