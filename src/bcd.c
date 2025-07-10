#include "bcd.h"
#include "bg_clip.h"

#define BCD_GET_FIRST(bcd) ((bcd) & 0x0F)
#define BCD_GET_SECOND(bcd) ((bcd) >> 4)

Bcd BcdCreate(u8 number)
{
    u8 first;
    u8 second;

    first = 0;
    second = 0;

    while (number > 9)
    {
        number -= 10;
        second++;
    }

    first = number;

    return first | second << 4;
}

Bcd BcdAdd(Bcd number, u8 add)
{
    u8 first;
    u8 second;

    first = BCD_GET_FIRST(number) + add;
    second = BCD_GET_SECOND(number);

    while (first > 9)
    {
        first -= 10;
        second++;
    }

    return first | second << 4;
}

void BcdDraw(u8 x, u8 y, Bcd number)
{
    DrawNumber(x, y, BCD_GET_FIRST(number));

    if (BCD_GET_SECOND(number) != 0)
        DrawNumber(x - 1, y, BCD_GET_SECOND(number));
}
