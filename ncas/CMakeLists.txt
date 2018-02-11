set(Sources
src/main.c
src/lexer.c
)

add_executable(ncas ${Sources} ${CommonSources})

target_include_directories(ncas PRIVATE include ../common/include)
target_compile_definitions(ncas PRIVATE _POSIX_C_SOURCE=200112L)
set_target_properties(ncas PROPERTIES
    COMPILE_FLAGS "-Werror -Wall -std=c89 -ansi"
)