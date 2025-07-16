    .module math

    .globl _ComputeIndexFromSpriteCoords

; Params :
;   de      : y
;   a       : width
;   sp[2]   : x
;
; Returns:
;   bc  : Index
_ComputeIndexFromSpriteCoords:
    ; 48 cycles, not counting the internal multiplication

    ; y / 32, use the y * 8 / 256 technique

    ld h, d
    ld l, e
    ; Save width for the mul function call later
    ld e, a
    add hl, hl
    add hl, hl
    add hl, hl
    ld a, h

    ; - 2
    dec a
    dec a

    ; * width
    call __muluchar
    ; Result of the multiplication is in bc

    ; x / 32 - 1
	ldhl sp,	#2
	ld a, (hl+)
	ld h, (hl)
    ld l, a
    add hl, hl
    add hl, hl
    add hl, hl

    ; Do some flipping in order to do hl = 00(h-1)
    ld a, h
    dec a
    ld l, a
    xor a
    ld h, a

    ; Perform final addition, and store in bc
    add hl, bc
    ld b, h
    ld c, l

    ; Pop return address
	pop	hl
    ; Pop old width parameter
	pop	af
    ; Return using a jump, since the stack has been tampered with
	jp (hl)
