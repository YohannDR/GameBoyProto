#include "sprites_AI/movable_object.h"

#include "gb/oam.h"

#include "sprite.h"

enum MovableObjectPose {
    POSE_IDLE = 1,
};

extern const struct AnimData sMovableObjectAnim[];

void MovableObject(void)
{
    if (gCurrentSprite.pose == 0)
    {
        gCurrentSprite.animPointer = sMovableObjectAnim;
        
        gCurrentSprite.drawDistanceLeft = SUB_PIXEL_TO_PIXEL(0);
        gCurrentSprite.drawDistanceRight = SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceTop = -SUB_PIXEL_TO_PIXEL(BLOCK_SIZE);
        gCurrentSprite.drawDistanceBottom = SUB_PIXEL_TO_PIXEL(0);

        gCurrentSprite.pose = POSE_IDLE;
    }
}

static const u8 sMovableObjectAnim_Frame0[OAM_DATA_SIZE(1)] = {
    1,
    OAM_POS(-8), OAM_POS(0), 0, 0,
};

const struct AnimData sMovableObjectAnim[] = {
    [0] = {
        .oamPointer = sMovableObjectAnim_Frame0,
        .duration = 255,
    },
    [1] = SPRITE_ANIM_TERMINATOR
};

const u8 sMovableObjectGraphics[] = {
    1,

    0xFF, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0xFF, 0xFF,
};
