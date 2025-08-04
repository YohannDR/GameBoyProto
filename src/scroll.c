#include "scroll.h"

#include "bg.h"
#include "player.h"

struct Camera gCamera;

static void CheckForTilemapUpdate(void)
{
    u8 blockX;
    u8 right;

    gTilemapUpdateDirection = TILEMAP_UPDATE_NONE;

    blockX = SUB_PIXEL_TO_BLOCK(gCamera.x);

    if (gCamera.left == blockX + 1)
    {
        gCamera.left--;
        gCamera.right--;
        gTilemapUpdateDirection = TILEMAP_UPDATE_LEFT;
        return;
    }

    right = blockX + SCREEN_SIZE_X_BLOCK;

    if (gCamera.right == right - 1)
    {
        gCamera.left++;
        gCamera.right++;
        gTilemapUpdateDirection = TILEMAP_UPDATE_RIGHT;
        return;
    }
}

static void UpdateCamera(void)
{
    // Apply camera velocity
    gCamera.x += gCamera.xVelocity;
    gCamera.y += gCamera.yVelocity;

    // Also apply it to the background
    gBackgroundInfo.x += SUB_PIXEL_TO_PIXEL(gCamera.xVelocity);
    gBackgroundInfo.y += SUB_PIXEL_TO_PIXEL(gCamera.yVelocity);

    CheckForTilemapUpdate();
}

void SetCameraPosition(u16 x, u16 y)
{
    gCamera.x = x;
    gCamera.y = y;

    gCamera.xVelocity = 0;
    gCamera.yVelocity = 0;

    gCamera.left = SUB_PIXEL_TO_BLOCK(x);
    gCamera.right = gCamera.left + SCREEN_SIZE_X_BLOCK;
    gCamera.top = SUB_PIXEL_TO_BLOCK(y);
    gCamera.bottom = gCamera.top + SCREEN_SIZE_Y_BLOCK;
}

void ScrollUpdate(void)
{
    gCamera.xVelocity = gPlayerMovement.xVelocity;

    UpdateCamera();
}
