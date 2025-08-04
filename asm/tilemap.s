	.module tilemap

    .globl _TilemapUpdateVblank

_TilemapUpdateVblank:
	ld	a, (#_gTilemapUpdateDirection)
	or	a, a
	ret	Z

	ld	a, (#_gTilemapUpdateDirection)
    dec a
    jr	Z, UpdateLeft

	ld	a, (#_gTilemapUpdateDirection)
    sub a, #0x02
    jr	Z, UpdateRight

	ld	a, (#_gTilemapUpdateDirection)
    sub a, #0x03
    jr	Z, UpdateTop

	ld	a, (#_gTilemapUpdateDirection)
    sub a, #0x04
    jr	Z, UpdateBottom

UpdateLeft:
    ; Get background X block position
	ld	a, (_gCamera + 6)
	ld	c, a
	ld	b, #0x0

    ; Get tilemap pointer (in de)
    ld hl, #(_gTilemap)
    ld	a, (hl+)
	ld	e, a
	ld	d, (hl)
    ld h, d
    ld l, e
    add hl, bc
    ld d, h
    ld e, l

    ; Get vram dst address (in bc)
    ld hl, #(0x9800)
    ld a, c
    and a, #0x1F
    ld c, a
    add hl, bc
    ld b, h
    ld c, l

	ld	hl, #_gTilemap + 4
	ld	(hl), #0x12

0$:
    ld a, (de)
    ld (bc), a

    ld	hl, #_gTilemap + 4
	dec (hl)
    ret Z

    ld hl, #0x0020
    add hl, bc
    ld b, h
    ld c, l

    ; Get tilemap width
    ld a, (#(_gTilemap + 2))
    ld l, a
    ld h, #0x00
    add hl, de
    ld d, h
    ld e, l

    jr 0$

UpdateRight:
    ; Get background X block position
	ld	a, (_gCamera + 7)
	ld	c, a
	ld	b, #0x0

    ; Get tilemap pointer (in de)
    ld hl, #(_gTilemap)
    ld	a, (hl+)
	ld	e, a
	ld	d, (hl)
    ld h, d
    ld l, e
    add hl, bc
    ld d, h
    ld e, l

    ; Get vram dst address (in bc)
    ld hl, #(0x9800)
    ld a, c
    and a, #0x1F
    ld c, a
    add hl, bc
    ld b, h
    ld c, l

	ld	hl, #_gTilemap + 4
	ld	(hl), #0x12

0$:
    ld a, (de)
    ld (bc), a

    ld	hl, #_gTilemap + 4
	dec (hl)
    ret Z

    ld hl, #0x0020
    add hl, bc
    ld b, h
    ld c, l

    ; Get tilemap width
    ld a, (#(_gTilemap + 2))
    ld l, a
    ld h, #0x00
    add hl, de
    ld d, h
    ld e, l

    jr 0$

; TODO vertical scrolling
UpdateTop:
    ret
UpdateBottom:
    ret
