	.module intr

    .globl _EnableInterrupts
	.globl _DisableInterrupts

    .area _CODE

_EnableInterrupts::
    reti

_DisableInterrupts::
    di
    ret
