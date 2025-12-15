#include "scroll.h"

#include "bg.h"
#include "game_state.h"
#include "room.h"
#include "player.h"

struct Camera gCamera;

#define SCROLL_VELOCITY_CAP (HALF_BLOCK_SIZE)

static void UpdateCamera(void)
{
    // Apply camera velocity
    gCamera.x += gCamera.xVelocity;
    gCamera.y += gCamera.yVelocity;

    // Also apply it to the background
    gBackgroundInfo.x += gCamera.xVelocity;
    gBackgroundInfo.y += gCamera.yVelocity;

    gBackgroundInfo.blockX = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.x);
    gBackgroundInfo.blockY = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.y);

    gBackgroundX = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    gBackgroundY = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);
}

u16 GetCameraTargetX(void)
{
    u16 playerX;
    u16 width;

    playerX = gPlayerData.x + BLOCK_SIZE;
    width = gTilemap.width * BLOCK_SIZE;

    // Check is on the far left of the scroll, i.e. if the distance between the start and the coords X is smaller than the anchor
    if (playerX < SCROLL_X_ANCHOR)
    {
        // Screen should be at the left limit of the scroll then
        return 0;
    }

    // Check isn't on the far right of the scroll, i.e. if the distance between the end and the coords X is smaller than the anchor
    if (playerX <= width - SCROLL_X_ANCHOR)
    {
        // In the middle of the scroll otherwhise, set the position to the coords - anchor
        return playerX - SCROLL_X_ANCHOR;
    }

    // Screen should "stop" before the right limit, so set it to right - screen size
    return width - SCREEN_SIZE_X_SUB_PIXEL;
}

u16 GetCameraTargetY(void)
{
    u16 playerY;
    u16 height;

    playerY = gPlayerData.y;
    height = gTilemap.height * BLOCK_SIZE;

    // Check is on the far top of the scroll, i.e. if the distance between the start and the coords Y is smaller than the anchor
    if (playerY < SCROLL_Y_ANCHOR)
    {
        // Screen should be at the top limit of the scroll then
        return 0;
    }

    // Check isn't on the bottom of the scroll, i.e. if the distance between the end and the coords Y is smaller than the anchor
    if (playerY <= height - SCROLL_Y_ANCHOR)
    {
        // In the middle of the scroll otherwhise, set the position to the coords - anchor
        return playerY - SCROLL_Y_ANCHOR;
    }

    // Screen should "stop" before the bottom limit, so set it to bottom - screen size
    return height - SCREEN_SIZE_Y_SUB_PIXEL;
}

void ComputeScroll(void)
{
    gCamera.xVelocity = GetCameraTargetX() - gCamera.x;

    if (gCamera.xVelocity < 0)
    {
        if (gCamera.xVelocity < -SCROLL_VELOCITY_CAP)
            gCamera.xVelocity = -SCROLL_VELOCITY_CAP;
    }
    else
    {
        if (gCamera.xVelocity > SCROLL_VELOCITY_CAP)
            gCamera.xVelocity = SCROLL_VELOCITY_CAP;
    }

    gCamera.yVelocity = GetCameraTargetY() - gCamera.y;

    if (gCamera.yVelocity < 0)
    {
        if (gCamera.yVelocity < -SCROLL_VELOCITY_CAP)
            gCamera.yVelocity = -SCROLL_VELOCITY_CAP;
    }
    else
    {
        if (gCamera.yVelocity > SCROLL_VELOCITY_CAP)
            gCamera.yVelocity = SCROLL_VELOCITY_CAP;
    }
}

void SetCameraPosition(u16 x, u16 y)
{
    gCamera.x = x;
    gCamera.y = y;

    gBackgroundInfo.x = x;
    gBackgroundInfo.y = y;

    gBackgroundInfo.blockX = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.x);
    gBackgroundInfo.blockY = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.y);

    gBackgroundX = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.x);
    gBackgroundY = SUB_PIXEL_TO_PIXEL(gBackgroundInfo.y);

    gCamera.xVelocity = 0;
    gCamera.yVelocity = 0;

    gCamera.left = SUB_PIXEL_TO_BLOCK(x);
    gCamera.right = gCamera.left + SCREEN_SIZE_X_BLOCK + 1;
    gCamera.top = SUB_PIXEL_TO_BLOCK(y) - 1;
    gCamera.bottom = SUB_PIXEL_TO_BLOCK(y) + SCREEN_SIZE_Y_BLOCK + 1;
}

void ScrollUpdate(void)
{
    ComputeScroll();
    UpdateCamera();
}
