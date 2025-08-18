    .module sprite

    .globl _UpdateSpritesAsm
    .globl _SpriteUpdateAnimationAsm

_SpriteUpdateAnimationAsm:
    ; Get current animation frame
    ld a, (_gCurrentSprite + 9)
    ld c, a
    ; Multiply by 3
    add a, a
    add a, c
    ld c, a
    ld b, #0x00

    ; Get anim pointer in hl
    ld hl, #_gCurrentSprite + 11
    ld a, (hl+)
    ld h, (hl)
    ld l, a

    ; Index anim pointer using bc
    add hl, bc

    ; Offset into timer field
    inc hl
    inc hl

    ; Increment animation timer
    ld bc, #_gCurrentSprite + 10
    ld a, (bc)
    inc a
    ; Save this in e, we'll use it later and it's less expensive than reloading it
    ld e, a
    ld (bc), a

    ; Load frame duration, we can increment hl here once, if doesn't cost an additinal cycle and we may need to increase the pointer later
    ld a, (hl+)
    ; Compare frame duration and current timer
    sub a, e
    ret NC

    ; Reset animation timer
    xor a, a
    ld (bc), a

    ; Decrement to reach the current frame field
    dec bc

    ; Increment current frame
    ld a, (bc)
    inc a
    ld (bc), a

    ; Goto next frame in the anim pointe
    inc hl
    inc hl

    ; Get duration of the next frame
    ld a, (hl)

    ; Check is 0
    or a, a
    ret NZ

    ; Reset animation frame to loop the animation
    ld (bc), a

    ; Set the anim ended flag
    ld hl, #_gCurrentSprite
    set 3, (hl)

    ret

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
    call _SpriteUpdateAnimationAsm

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
