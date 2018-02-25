cmake_minimum_required(VERSION 3.3)
project(virtualpc C)

set(CommonSources
        ../common/src/array.c
        ../common/src/registers.c
        )

set(CommonHeaders
        ../common/include/registers.h
        ../common/include/cvm/cvm.h
        ../common/include/array.h
        ../common/include/instruction.h
        )

add_subdirectory(pc)
add_subdirectory(ncas)