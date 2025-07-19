	.module room

    .globl _LoadGraphics

; Params :
;   de      : graphics pointer
_LoadGraphics:
    ld a, (de)
    inc de
    ld l, a
    ld h, #0x00
    add	hl, hl
	add	hl, hl
	add	hl, hl
	add	hl, hl
    ld	bc, #0x9000
0$:
    ld a, (de)
    ld (bc), a

    inc de
    inc bc

    dec hl
    ld	a, l
	or	a, h
    jr NZ, 0$

    ret