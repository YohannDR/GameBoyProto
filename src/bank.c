#include "bank.h"

#include "gb/io.h"

#include "io.h"

u8 gCurrentBank;
u8 gPreviousBank;

void BankInit(void)
{
    gCurrentBank = BANK_DATA;
    SwitchBank(gCurrentBank);
}

void SwitchBank(u8 bank)
{
    gPreviousBank = gCurrentBank;
    gCurrentBank = bank;

    Write8(REG_BANK0, bank);
}

void BankGoBack(void)
{
    SwitchBank(gPreviousBank);
}
