    .module transition

    .globl _UpdateGraphicsLoader

; This function is time critical, and has to be perfectly optimized to run as fast as possible
; this allows us to barely have enough time to load 4 tiles
_UpdateGraphicsLoader:
    ; Check whether the loader is busy
    ld a, (#_gGraphicsLoaderInfo + 0)
    or a, a
    ret Z

    ; Get the vram addr in bc
    ld hl, #(_gGraphicsLoaderInfo + 1)
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a

    ; Get buffer address
    ld hl, #(_gGraphicsLoaderBuffer)

    ; Get amount of bytes to transfer
    ld a, (#_gGraphicsLoaderInfo + 7)
    or a, a
    ret Z
    ld e, a

LoadGfxLoop:
    ; Perform the copy
    ld a, (hl+)
    ld (bc), a

    ; Decrement counter
    dec e

    ; Check if we're done
    ret Z

    ; Increment vram address
    inc bc

    jr LoadGfxLoop

    ret
