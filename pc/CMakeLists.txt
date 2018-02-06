set(Sources
src/mem.c
src/cpu/cat/cat.c
src/cpu.c
src/vio/vio.c
src/vio/dev/term.c
src/main.c
src/pc.c
src/bus.c)

add_executable(pc ${Sources})

target_include_directories(pc PRIVATE include)
target_compile_definitions(pc PRIVATE _POSIX_C_SOURCE=200112L)
set_target_properties(pc PROPERTIES
    COMPILE_FLAGS "-Werror -Wall -std=c89"
)