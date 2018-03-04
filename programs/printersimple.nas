; The goal is to make this file compile-able
; And the resulting program must work on the virtual PC
; The string to be printed must be located at 100100h

origin 100000h

label reset
	xor ra, ra                      ; reset ra
label loop
	cmp [60h], 0                    ; vio[0].r0: is device ready?
	jnz loop                        ; loop until terminal is ready
	mov byte [80h], [ra + 100100h]  ; vio[0].data[0]: set the message (single character)
	mov [64h], 1                    ; vio[0].r1: length of message = 1
	mov [60h], 1                    ; vio[0].r0: VDCS_TERM_PRINT
                            	    ; This actually sends the command to terminal to print the message. r0 is then reset by the device.
	cmp byte [ra + 100100h], 0      ; if end of string is reached (null character)
	jz reset                        ; jump to iterator reset
	inc ra                          ; iterator++
	jmp loop                        ; go back to loop