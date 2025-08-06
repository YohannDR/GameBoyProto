#include "scroll.h"

#include "bg.h"
#include "game_state.h"
#include "room.h"
#include "player.h"

struct Camera gCamera;

#define SCROLL_VELOCITY_CAP (HALF_BLOCK_SIZE)

static void CheckForTilemapUpdate(void)
{
    u8 blockX;

    gTilemapUpdateDirection = TILEMAP_UPDATE_NONE;

    blockX = SUB_PIXEL_TO_BLOCK(gCamera.x);

    if (gCamera.xVelocity < 0 && gCamera.left == blockX)
    {
        gCamera.left--;
        gCamera.right--;
        SetupTilemapUpdate(TILEMAP_UPDATE_LEFT);
        return;
    }

    if (gCamera.xVelocity > 0 && gCamera.right == blockX + SCREEN_SIZE_X_BLOCK - 1)
    {
        gCamera.left++;
        gCamera.right++;
        SetupTilemapUpdate(TILEMAP_UPDATE_RIGHT);
        return;
    }
}

static void UpdateCamera(void)
{
    // Apply camera velocity
    gCamera.x += gCamera.xVelocity;
    gCamera.y += gCamera.yVelocity;

    // Also apply it to the background
    gBackgroundInfo.x += gCamera.xVelocity;
    gBackgroundInfo.y += gCamera.yVelocity;

    CheckForTilemapUpdate();
}

static u16 GetCameraTargetX(void)
{
    u16 playerX;
    u16 width;

    playerX = gPlayerData.x + BLOCK_SIZE - gRoomOriginX;
    width = gTilemap.width * BLOCK_SIZE;

    // Check is on the far left of the scroll, i.e. if the distance between the start and the coords X is smaller than the anchor
    if (playerX < SCREEN_SIZE_X_SUB_PIXEL / 2)
    {
        // Screen should be at the left limit of the scroll then
        return 0;
    }

    // Check isn't on the far right of the scroll, i.e. if the distance between the end and the coords X is smaller than the anchor
    if (playerX <= width - SCREEN_SIZE_X_SUB_PIXEL / 2)
    {
        // In the middle of the scroll otherwhise, set the position to the coords - anchor
        return playerX - SCREEN_SIZE_X_SUB_PIXEL / 2;
    }

    // Screen should "stop" before the right limit, so set it to right - screen size
    return width - SCREEN_SIZE_X_SUB_PIXEL;
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
}

void SetCameraPosition(u16 x, u16 y)
{
    gCamera.x = x;
    gCamera.y = y;

    gCamera.xVelocity = 0;
    gCamera.yVelocity = 0;

    gCamera.left = SUB_PIXEL_TO_BLOCK(x);
    gCamera.right = gCamera.left + SCREEN_SIZE_X_BLOCK + 1;
    gCamera.top = SUB_PIXEL_TO_BLOCK(y);
    gCamera.bottom = gCamera.top + SCREEN_SIZE_Y_BLOCK;
}

void ScrollUpdate(void)
{
    if (gGameMode.main == GM_IN_GAME)
    {
        ComputeScroll();
        UpdateCamera();

        gBackgroundInfo.blockX = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.x);
        gBackgroundInfo.blockY = SUB_PIXEL_TO_BLOCK(gBackgroundInfo.y);
    }
}
