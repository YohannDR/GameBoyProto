    .module tilemap

    .globl _TilemapUpdateVblank

_TilemapUpdateVblank:
    ld a, (#_gTilemapUpdateDirection)
    or a, a
    ret Z

    ld a, (#_gTilemapUpdateDirection)
    dec a
    jr Z, UpdateLeft

    ld a, (#_gTilemapUpdateDirection)
    sub a, #0x02
    jr Z, UpdateRight

    ld a, (#_gTilemapUpdateDirection)
    sub a, #0x03
    jr Z, UpdateTop

    ld a, (#_gTilemapUpdateDirection)
    sub a, #0x04
    jr Z, UpdateBottom

UpdateLeft:
    ; Get background X block position
    ld a, (_gBackgroundInfo + 4)
    ld c, a
    ld b, #0x0

    jr UpdateHorizontal

UpdateRight:
    ; Get background X block position
    ld a, (_gBackgroundInfo + 4)
	add a, #0x15
    ld c, a
    ld b, #0x0

UpdateHorizontal:
    ; Get vram dst address (in bc)
    ld hl, #(0x9800)
    ld a, c
    and a, #0x1F
    ld c, a
    add hl, bc
    ld b, h
    ld c, l

    ; Get tilemap pointer (in de)
    ld de, #(_gTilemapUpdateBuffer)

    ; Use reserved tilemap byte as loop counter
    ld hl, #_gTilemap + 4
    ld (hl), #0x12

UpdateHorizontalLoop:
    ld a, (de)
    ld (bc), a

    ; Check counter
    ld hl, #_gTilemap + 4
    dec (hl)
    ret Z

    ; Advance VRAM pointer
    ld hl, #0x0020
    add hl, bc
    ld b, h
    ld c, l

    ; Advance buffer pointer
    ; Since the buffer is somewhat small, we might be able to get away with just increasing e
    ; but that would require the buffer to have a compatible RAM address, so this'll have to be investigated latter
    ; near the end of production once RAM is basically fully determined
    inc de

    jr UpdateHorizontalLoop

; TODO vertical scrolling
UpdateTop:
    ret
UpdateBottom:
    ret
