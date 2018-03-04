set(Sources
        src/main.c
         src/mnemonics.c src/cutter.c src/analyzer.c src/model.c src/parser.c src/compiler.c)

set(Headers
        include/mnemonics.h ../common/include/iset/micro.h include/cutter.h include/analyzer.h include/model.h include/compiler.h include/parser.h)

add_executable(ncas ${Sources} ${CommonSources} ${Headers} ${CommonHeaders})

target_include_directories(ncas PRIVATE include ../common/include)
target_compile_definitions(ncas PRIVATE _POSIX_C_SOURCE=200112L)
set_target_properties(ncas PROPERTIES
        COMPILE_FLAGS "-Werror -Wall -Wextra -ansi"
        )