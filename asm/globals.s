    .module globals

    .globl  .STACK
    .globl  _shadow_OAM
    .globl  .refresh_OAM
    .globl  ___memcpy
    .globl  __muluchar

; Params :
;       bc    : Source address
;       de    : Destination address
;       sp[2] : Size
___memcpy:
    ; Retrieve size, it's after the return address so we need to load + 2
    ldhl sp, #2
    ; a = Size
    ld a, (hl)
    ld h, a
$0:
    ; Get value from source
    ld a, (bc)
    ; Write value to destination
    ld (de), a

    ; Incremnt src and dst
    inc bc
    inc de

    ; Decrement counter
    dec h
    ; Loop if the counter hasn't reached 0
    jp nz, $0

    ; Fix up stack, this removes the size parameter from the stack

    ; Pop return address
    pop bc
    ; Pop size
    pop de
    ; Push return address
    push bc
    ret

__muluchar:
        ld      c, a
        xor     a
        ;; Clear the top
        ld      d, a

        ;; Version that uses an 8bit multiplicand
        ;;
        ;; Entry conditions
        ;;   C  = multiplicand
        ;;   DE = multiplier
        ;;   A  = 0
        ;;
        ;; Exit conditions
        ;;   BC = less significant word of product
        ;;
        ;; Register used: AF,BC,DE,HL
.mul8:
        ld      l,a
        ld      b,#8
        ld      a,c
loop8:
        add     hl,hl
        rla
        jr      NC,skip8
        add     hl,de
skip8:
        dec     b
        jr      NZ,loop8

        ;; Return in bc
        ld      c,l
        ld      b,h

        ret
