instruction [size] [operand[, operand]] ; comment

use of labels:

:label  ; sets label at current position

jmp label ; absolute jump to label
jmp -label ; relative jump to label (not implemented)

size: byte, word, dword