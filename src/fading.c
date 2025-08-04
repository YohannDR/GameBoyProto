#include "fading.h"

#include "gb/io.h"
#include "gb/display.h"

#include "io.h"

u8 gBackgroundPalette;
u8 gObj0Palette;
u8 gObj1Palette;
struct FadingInfo gColorFading;

static void FadingApply(u8* pal, u8 target)
{
    u8 palette;
    u8 newPalette;
    u8 color;
    u8 i;
    u8 finishedColors;
    u8 targetColor;
    u8 targetPalette;

    // Verify that if this target is actually active
    if (!(gColorFading.target & target))
        return;

    // Fetch the target palette
    if (target == FADING_TARGET_BACKGROUND)
        targetPalette = gColorFading.targetPalettes[0];
    else if (target == FADING_TARGET_OBJ0)
        targetPalette = gColorFading.targetPalettes[1];
    else if (target == FADING_TARGET_OBJ1)
        targetPalette = gColorFading.targetPalettes[2];
    else
        targetPalette = 0;

    // Get the current palette
    palette = Read8(pal);

    newPalette = 0;
    finishedColors = 0;

    // Look for each color in the palette
    for (i = 0; i < 4; i++)
    {
        // Grab the color for both the current and target
        color = palette & 0b11;
        targetColor = targetPalette & 0b11;

        // Compare both colors
        if (color == targetColor)
            finishedColors++;
        else if (color < targetColor)
            color++;
        else if (color > targetColor)
            color--;

        // Advance both palettes
        palette >>= 2;
        targetPalette >>= 2;

        // And add the new color to the new palette
        // This could have been newPalette |= color << (i * 2) but for some reason this makes the compiler freak out
        // and output incorrect asm, it allocated finishedColors to register l, but it was overwritten, so the check at
        // the end of the function stopped working
        if (i == 0)
            newPalette |= color << 0;
        else if (i == 1)
            newPalette |= color << 2;
        else if (i == 2)
            newPalette |= color << 4;
        else if (i == 3)
            newPalette |= color << 6;
    }

    // Apply the new palette
    Write8(pal, newPalette);

    if (finishedColors == 4)
    {
        // If all 4 colors are the same, then we can disable this target since it finished
        gColorFading.target &= ~target;
    }
}

void FadingStart(u8 target, u8 color, u8 interval)
{
    gColorFading.target |= target;
    gColorFading.timer = 0;
    gColorFading.interval = interval;

    if (target & FADING_TARGET_BACKGROUND)
        gColorFading.targetPalettes[0] = color;

    if (target & FADING_TARGET_OBJ0)
        gColorFading.targetPalettes[1] = color;

    if (target & FADING_TARGET_OBJ1)
        gColorFading.targetPalettes[2] = color;
}

void FadingUpdate(void)
{
    if (gColorFading.target == FADING_TARGET_NONE)
        return;

    gColorFading.timer++;

    if (gColorFading.timer != gColorFading.interval)
        return;

    gColorFading.timer = 0;

    FadingApply((u8*)REG_BGP, FADING_TARGET_BACKGROUND);
    FadingApply((u8*)REG_OBP0, FADING_TARGET_OBJ0);
    FadingApply((u8*)REG_OBP1, FADING_TARGET_OBJ1);
}
