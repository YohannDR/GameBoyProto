    .module globals

    .globl  .STACK
    .globl  _shadow_OAM
    .globl  .refresh_OAM
    .globl  ___memcpy
    .globl  __muluchar

; Params :
;   bc    : Source address
;   de    : Destination address
;   sp[2] : Size
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
