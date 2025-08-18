    .module sprite

    .globl _UpdateSpritesAsm

_UpdateSpritesAsm:
    ld hl, #(_gSpriteData)
    ld a, #20
    ld (_gEnemiesLeftToProcess), a

.loopStart:
    ; Get status
    ld a, (hl)
    rrca
    jr NC, .loopContinue

    ; Copy sprite to gCurrentSprite
    ld bc, #_gCurrentSprite
    ld e, #0x17

    .copy1Start:
        ld a, (hl+)
        ld (bc), a
        inc c
        dec e
    jr NZ, .copy1Start

    ; Clear anim ended flag
    ld a, (#_gCurrentSprite + 0)
    res 3, a
    ld (#_gCurrentSprite + 0), a

    ; Save hl
    push hl

    call _SpriteUpdateOnScreenFlag
    call _SpriteUpdateAnimation

    ; Call AI
    ld a, (#_gCurrentSprite + 5)
    add a, a
    ld c, a
    ld b, #0
    ld hl, #_sSpriteAiPointers
    add hl, bc
    ld a, (hl+)
    ld c, a
    ld h, (hl)
    ld l, c
    call ___sdcc_call_hl

    ; Check should draw
    ld a, (#_gCurrentSprite + 0)
    and a, #0x07
    sub a, #0x03
    jr NZ, .loopContinueCopy

    call _SpriteDraw

.loopContinueCopy:
    ; Restore hl
    pop hl
    ; hl is currently at the start of the next sprite slot due to the previous copy, so we need to move down one byte
    dec l

    ; This time the copy is backwards due to hl being at the end
    ld bc, #(_gCurrentSprite + 0x17 - 1)
    ld e, #0x17
    .copy2Start:
        ld a, (bc)
        ld (hl-), a
        dec c
        dec e
    jr NZ, .copy2Start
    inc l

.loopContinue:
    ; Decrement counter and check if we're done
    ld a, (_gEnemiesLeftToProcess)
    dec a
    ld (_gEnemiesLeftToProcess), a
    ret Z

    ; Increment sprite pointer
    ld bc, #0x17
    add hl, bc

    jr .loopStart
