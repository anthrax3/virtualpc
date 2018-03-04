; The goal is to make this file compile-able
; And the resulting program must work on the virtual PC
; This file will be compiled into linkable object file

const BaseAddress dword 100000h
const MessageAddress dword 100100h

origin MessageAddress

: message
    store "Hello, world", byte 0h

origin BaseAddress

: reset
    xor ra, ra                          ; reset ra
: loop
    cmp [60h], 0                        ; vio[0].r0: is device ready?
    jnz loop                            ; loop until terminal is ready
    mov [80h], [ra + MessageAddress]    ; vio[0].data[0]: set the message (single character)
    mov [64h], 1                        ; vio[0].r1: length of message = 1
    mov [60h], 1                        ; vio[0].r0: VDCS_TERM_PRINT
                                        ; This actually sends the command to terminal to print the message. r0 is then reset by the device.
    cmp [ra + MessageAddress], 0        ; if end of string is reached (null character)
    jz reset                            ; jump to iterator reset
    inc ra                              ; iterator++
    jmp loop                            ; go back to loop