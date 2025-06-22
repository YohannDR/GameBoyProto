	.module intr

.macro PUSH_ALL
    push af
    push bc
    push de
    push hl
.endm

.macro POP_ALL
    pop af
    pop bc
    pop de
    pop hl
.endm

    .area _CODE

_EnableInterrupts::
    reti

_DisableInterrupts::
    di
    ret

_VblankHandler::
    PUSH_ALL

    call _CallbackCallVblank

    POP_ALL
    reti

_LcdHandler::
    PUSH_ALL

    call _CallbackCallLcd

    POP_ALL
    reti

_TimerHandler::
    PUSH_ALL

    call _CallbackCallTimer

    POP_ALL
    reti
