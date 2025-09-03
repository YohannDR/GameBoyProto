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

    ld a, (_gIsIdleFrame)
    or a, a
    jr NZ, end

    ; We also need to update oam during v-blank
    call .refresh_OAM
    ; This is time critical, so it takes absolute priority
    call _UpdateTransitionVblank
    call _TilemapUpdateVblank
    call _ApplyBgChanges
    call _CallbackCallVblank

end:
    ld a, (_gIsIdleFrame)
    cpl
    ld (_gIsIdleFrame), a
    POP_ALL
    reti

_LcdHandler::
    PUSH_ALL

    call _CallbackCallLcd

    POP_ALL
    reti
