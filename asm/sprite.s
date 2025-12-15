    .module sprite

    .globl _UpdateSprites
    .globl _SpriteUpdateAnimation
    .globl _SpriteDraw
    .globl _SpriteUpdateOnScreenFlag

_SpriteComputeCameraPosition:
    ld hl, #_gCurrentSprite + 1
    ld a, (hl+)
    ld c, a
    ld b, (hl)
    srl b
    rr c
    srl b
    rr c
    ld hl, #_gBackgroundInfo
    ld a, (hl+)
    ld b, a
    ld e, (hl)
    srl e
    rr b
    srl e
    rr b
    ld a, c
    sub a, b
    ld (#_gSpriteScreenX), a

    ld hl, #_gCurrentSprite + 3
    ld a, (hl+)
    ld c, a
    ld b, (hl)
    srl b
    rr c
    srl b
    rr c
    ld hl, #_gBackgroundInfo + 2
    ld a, (hl+)
    ld b, (hl)
    ld e, a
    srl b
    rr e
    srl b
    rr e
    ld a, c
    sub a, e
    ld (#_gSpriteScreenY), a
    ret

_SpriteUpdateOnScreenFlag:
    ; Clear the on screen flag
    ld a, (_gCurrentSprite + 0)
    res 1, a
    ld (_gCurrentSprite + 0), a

    ; Get background X in de
    ld hl, #_gBackgroundX
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a
    ld hl, #8
    add hl, bc
    ld e, l
    ld d, h

    ; Get sprite X in pixels
    ld hl, #(_gCurrentSprite + 1)
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a
    srl b
    rr c
    srl b
    rr c

    ; Get right bound
    ld a, (_gCurrentSprite + 16)
    ld l, a
    ld h, #0

    add hl, bc

    ; Compare right and screen X
    ld a, e
    sub a, l
    ld a, d
    sbc a, h
    ret NC

    ; Get left bound, X screen position is still in l
    ld a, (_gCurrentSprite + 15)
    ld l, a
    ld h, #0
    add hl, bc
    ld c, l
    ld b, h

    ; Add screen size X
    ld hl, #160
    add hl, de

    ; Compare left and screen X
    ld a, c
    sub a, l
    ld a, b
    sbc a, h
    ret NC

    ; Now do the same but for Y

    ; Get background Y in de
    ld hl, #_gBackgroundY
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a
    ld hl, #16
    add hl, bc
    ld e, l
    ld d, h

    ; Get sprite Y in pixels
    ld hl, #(_gCurrentSprite + 3)
    ld a, (hl+)
    ld c, a
    ld a, (hl)
    ld b, a
    srl b
    rr c
    srl b
    rr c

    ; Get bottom bound
    ld a, (_gCurrentSprite + 18)
    ld l, a
    ld h, #0

    add hl, bc

    ; Compare bottom and screen Y
    ld a, e
    sub a, l
    ld a, d
    sbc a, h
    ; ret NC

    ; Get top bound
    ld a, (_gCurrentSprite + 17)
    ld l, a
    ld h, #0
    add hl, bc
    ld c, l
    ld b, h

    ; Add screen size Y
    ld hl, #144
    add hl, de

    ; Compare top and screen Y
    ld a, c
    sub a, l
    ld a, b
    sbc a, h
    ; ret NC

    ; Set on screen flag
    ld a, (_gCurrentSprite + 0)
    set 1, a
    ld (_gCurrentSprite + 0), a
    ret

_SpriteDraw:
    ld a, (_gNextOamSlot)
    ; * 4
    add a, a
    add a, a
    ld c, a
    ld b, #0

    ; Index oam buffer
    ld hl, #_gOamBuffer
    add hl, bc

    push hl

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

    ; Load oam pointer in bc
    ld a, (hl+)
    ld c, a
    ld b, (hl)

    ; Get part counter
    ld a, (bc)
    inc bc
    ld (_gOamPartsLeftToProcess), a

    ; Update next oam slot
    ld h, a
    ld a, (_gNextOamSlot)
    add a, h
    ld (_gNextOamSlot), a

    ; Prepare the sprite properties
    ld a, (_gCurrentSprite + 13)
    swap a
    and a, #0xF0
    ld (_gSpriteDrawAttributes), a

    pop hl

    ; At this point :
    ; 
    ; bc = src oam pointer
    ; hl = dst oam pointer
    ; d  = Y offset
    ; e  = X offset
    ld a, (_gSpriteScreenY)
    ld d, a
    ld a, (_gSpriteScreenX)
    ld e, a

    .drawLoopStart:
        ; Byte 0, Y position
        ld a, (bc)
        add a, d
        ld (hl+), a
        inc bc

        ; Byte 1, X position
        ld a, (bc)
        add a, e
        ld (hl+), a
        inc bc

        ; Byte 2, tile index
        push hl
        ld hl, #(_gCurrentSprite + 14)
        ld l, (hl)
        ld a, (bc)
        inc bc
        add a, l
        pop hl
        ld (hl+), a

        ; Byte 3, attributes
        push hl
        ld hl, #_gSpriteDrawAttributes
        ld a, (bc)
        inc bc
        ; Xor the object attribute with the sprite attributes
        xor (hl)
        pop hl
        ld (hl+), a

        ld a, (_gOamPartsLeftToProcess)
        dec a
        ret Z

        ld (_gOamPartsLeftToProcess), a
    jr .drawLoopStart

_SpriteUpdateAnimation:
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

_UpdateSprites:
    ld a, (_gMaxAmountOfExistingSprites)
    ld (_gEnemiesLeftToProcess), a

    ; Abort if there's no sprites to process
    or a, a
    ret Z

    ld hl, #(_gSpriteData)

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

    ; Save hl
    push hl

    ; Clear anim ended flag
    ld a, (#_gCurrentSprite + 0)
    res 3, a
    ld (#_gCurrentSprite + 0), a

    call _SpriteUpdateAnimation

    ; Check current game mode
    ld a, (_gGameMode + 0)
    or a, a
    jr Z, .callAi

    ; We ignore the game mode restriction and call AI at least once to initialize sprites
    ld a, (_gCurrentSprite + 8)
    or a, a
    jr Z, .callAi

    ; Only draw the sprites if we aren't in game
    call _SpriteComputeCameraPosition
    jr .skipAi

.callAi:
    ; Check for the disabled bit
    bit 6, a
    jr NZ, .skipAi

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

.skipAi:
    ; Compute the screen coords of the sprite
    call _SpriteComputeCameraPosition

    call _SpriteUpdateOnScreenFlag

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
