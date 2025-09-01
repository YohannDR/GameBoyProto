    .module room

    .globl _LoadGraphics

; Params :
;   de      : graphics pointer
_LoadGraphics:
    ld a, (de)
    inc de
    ld l, a
    ld h, #0x00
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    ld bc, #0x9000
0$:
    ld a, (de)
    ld (bc), a

    inc de
    inc bc

    ; Handle wrap around from 9800 to 8800
    ld a, b
    sub a, #0x98
    jr NZ, 1$

    ld b, #0x88

1$:
    dec hl
    ld a, l
    or a, h
    jr NZ, 0$

    ret
