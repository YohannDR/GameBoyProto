    .module tilemap

    .globl _TilemapUpdateVblank

_TilemapUpdateVblank:
    ld a, (#_gTilemapUpdateVerticalDirection)
    or a, a
    jr Z, CheckUpdateVertical

    ld a, (#_gTilemapUpdateVerticalDirection)
    dec a
    jr Z, UpdateHorizontal

    ld a, (#_gTilemapUpdateVerticalDirection)
    sub a, #0x02
    jr Z, UpdateHorizontal

    jp CheckUpdateVertical

UpdateHorizontal:
    ; Get vram dst address (in bc)
    ld hl, #(_gTilemap + 7)
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a

    ; Get tilemap pointer (in de)
    ld de, #(_gTilemapUpdateBufferVertical)

    ; Use reserved tilemap byte as loop counter
    ld hl, #_gTilemap + 4
    ld (hl), #0x12 + 2

UpdateHorizontalLoop:
    ld a, (de)
    ld (bc), a

    ; Check counter
    ld hl, #_gTilemap + 4
    dec (hl)
    jr Z, CheckUpdateVertical

    ; Advance VRAM pointer
    ld hl, #0x0020
    add hl, bc
    ld a, h
    and a, #0xFB
    ld b, a
    ld c, l

    ; Advance buffer pointer
    ; Since the buffer is somewhat small, we might be able to get away with just increasing e
    ; but that would require the buffer to have a compatible RAM address, so this'll have to be investigated later
    ; near the end of production once RAM is basically fully determined
    inc de

    jr UpdateHorizontalLoop

CheckUpdateVertical:
    ld a, (#_gTilemapUpdateHorizontalDirection)
    sub a, #0x03
    jr Z, UpdateVertical

    ld a, (#_gTilemapUpdateHorizontalDirection)
    sub a, #0x04
    jr Z, UpdateVertical

    ret

UpdateVertical:
    ; Get vram dst address (in bc)
    ld hl, #(_gTilemap + 5)
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a

    ; Get tilemap pointer (in de)
    ld de, #(_gTilemapUpdateBufferHorizontal)

    ; Use reserved tilemap byte as loop counter
    ld hl, #_gTilemap + 4
    ld (hl), #0x14 + 3

UpdateVerticalLoop:
    ld a, (de)
    ld (bc), a

    ; Check counter
    ld hl, #_gTilemap + 4
    dec (hl)
    ret Z

    ; Advance VRAM pointer
    ld a, c
    and a, #0x1F
    inc a
    and a, #0x1F
    ld h, a
    ld a, c
    and a, #0xE0
    or a, h
    ld c, a

    ; Advance buffer pointer
    ; Since the buffer is somewhat small, we might be able to get away with just increasing e
    ; but that would require the buffer to have a compatible RAM address, so this'll have to be investigated later
    ; near the end of production once RAM is basically fully determined
    inc de

    jr UpdateVerticalLoop
