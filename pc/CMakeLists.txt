set(Sources
src/mem.c
src/cpu/cat/cat.c
src/cpu/cat/instructions_byte.c
src/cpu/cat/instructions_micro.c
src/cpu/operand.c
src/cpu.c
src/vio/vio.c
src/vio/dev/term.c
src/main.c
src/pc.c
src/bus.c)

set(Headers
include/mem.h
include/main.h
include/cpu.h
include/bus.h
include/vio/vio.h
include/vio/devicecat.h
include/vio/dev/term.h
include/exception.h
include/cpu/instruction.h
include/cpu/operand.h
include/cpu/cat/instructions_byte.h
include/cpu/cat/cat.h
include/cpu/cat/instructions_word.h
include/cpu/cat/instructions.h
include/cpu/cat/instructions_micro.h
include/cpu/cat/instructions_dword.h
include/pc.h
)

add_executable(pc ${Sources} ${CommonSources} ${Headers} ${CommonHeaders})

target_include_directories(pc PRIVATE include ../common/include)
target_compile_definitions(pc PRIVATE _POSIX_C_SOURCE=200112L)
set_target_properties(pc PROPERTIES
    COMPILE_FLAGS "-Werror -Wall -Wextra -ansi"
)