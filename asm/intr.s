    .module intr

.macro PUSH_ALL
    push af
    push bc
    push de
    push hl
.endm

.macro POP_ALL
    pop hl
    pop de
    pop bc
    pop af
.endm

    .area _CODE

; Define the handlers for the different interrupts
; They first push every register, then call the callback if there's one, then pop every register
; before returning with interrupts enabled again

_VblankHandler::
    PUSH_ALL

    ; We also need to update oam during v-blank
    call .refresh_OAM
    ; This is time critical, so it takes absolute priority
    call _UpdateGraphicsLoader
    call _ApplyBgChanges
    call _CallbackCallVblank

    ld hl, #_gVblankFired
    ld (hl), #0x01

    call _CallbackCallVblank

    POP_ALL
    reti

_LcdHandler::
    PUSH_ALL

    call _CallbackCallLcd

    POP_ALL
    reti
