        ;; ****************************************
        ;; Beginning of module
        ;; BANKED: checked
        .title  "Runtime"
        .module Runtime
        .area   _HEADER (ABS)

        ;; RST vectors
;       .org    0x00            ; empty

;       .org    0x08            ; empty

;       .org    0x10            ; empty

;       .org    0x18            ; empty

;       .org    0x20            ; empty

;       .org    0x28            ; empty

;       .org    0x30            ; empty

;       .org    0x38            ; empty

        ;; Hardware interrupt vectors
        .org    0x40            ; VBL
        jp _VblankHandler

        .org    0x48            ; LCD
        jp _LcdHandler

        .org    0x50            ; TIM
        jp _TimerHandler

;       .org    0x58            ; SIO

;       .org    0x60            ; JOY

        ;; GameBoy Header
        .org    0x100
.header:
        JR      CodeStart

        ;; Nintendo logo
        .org    0x104
        .byte   0xCE,0xED,0x66,0x66
        .byte   0xCC,0x0D,0x00,0x0B
        .byte   0x03,0x73,0x00,0x83
        .byte   0x00,0x0C,0x00,0x0D
        .byte   0x00,0x08,0x11,0x1F
        .byte   0x88,0x89,0x00,0x0E
        .byte   0xDC,0xCC,0x6E,0xE6
        .byte   0xDD,0xDD,0xD9,0x99
        .byte   0xBB,0xBB,0x67,0x63
        .byte   0x6E,0x0E,0xEC,0xCC
        .byte   0xDD,0xDC,0x99,0x9F
        .byte   0xBB,0xB9,0x33,0x3E

        ;; Title of the game
        .org    0x134
        .asciz  "Prototype"

        .org    0x144
        .byte   0,0,0

        ;; Cartridge type is ROM only
        .org    0x147
        .byte   0

        ;; ROM size is 32kB
        .org    0x148
        .byte   0

        ;; RAM size is 0kB
        .org    0x149
        .byte   0

        ;; Maker ID
        .org    0x14A
        .byte   0x00,0x00

        ;; Version number
        .org    0x14C
        .byte   0x01

        ;; Complement check
        .org    0x14D
        .byte   0x00

        ;; Checksum
        .org    0x14E
        .byte   0x00,0x00

        ;; ****************************************
        .org    0x150

ClearRam::
        ; Size of ram
        ld bc, #l__DATA
        ; Start of ram
        ld hl, #s__DATA
        ; Load 0 into e
        xor a
        ld e, a
0$:
        ; Write value
        ld (hl),e
        ; Advance pointer
        inc hl
        ; Decrement counter
        dec bc
        ld a, b
        ld d, c
        or d
        ; Loop if the counter hasn't reached 0
        jr nz,0$

        ret

        ;; Initialization code
CodeStart::
        ; Disable interrupts while we're setting things up
        di
        ; Initialize stack pointer
        ld sp, #.STACK
        ; Clear RAM
        call ClearRam

        ; Now that setup is done, we can safely re-enable interrupts
        ei

        ; Call the main function
        call _main

        ; Main has exited, so we can assume the game has finished running
Exit::
        ; Since it finished, just halt the cpu, we literally have nothing else to do
        halt
        nop
        ; The console can exit this halt if an interrupt happens, but we don't care, so just go back to doing nothing
        jr Exit

        ;; ****************************************

        ;; Ordering of segments for the linker
        ;; Code that really needs to be in bank 0
        .area   _HOME
        ;; Similar to _HOME
        .area   _BASE
        ;; Code
        .area   _CODE
        ;; #pragma bank 0 workaround
        .area   _CODE_0
        ;; Constant data
        .area   _LIT
;       ;; since _CODE_1 area base address is pre-defined in the linker from 0x4000,
;       ;; that moves initializer code and tables out of bank 0
;       .area   _CODE_1
        ;; Constant data, used to init _DATA
        .area   _INITIALIZER
        ;; Code, used to init _DATA
        .area   _GSINIT
        .area   _GSFINAL
        ;; Uninitialised ram data
        .area   _DATA
        .area   _BSS
        ;; Initialised in ram data
        .area   _INITIALIZED
        ;; For malloc
        .area   _HEAP
        .area   _HEAP_END

        .area   _DATA
