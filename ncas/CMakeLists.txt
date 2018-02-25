set(Sources
        src/main.c
        src/lexer.c
        src/compiler.c)

set(Headers
        include/lexer.h
        include/compiler.h
        include/compiler.h include/mnemonics.h ../common/include/iset/micro.h)

add_executable(ncas ${Sources} ${CommonSources} ${Headers} ${CommonHeaders})

target_include_directories(ncas PRIVATE include ../common/include)
target_compile_definitions(ncas PRIVATE _POSIX_C_SOURCE=200112L)
set_target_properties(ncas PROPERTIES
        COMPILE_FLAGS "-Werror -Wall -Wextra -ansi"
        )