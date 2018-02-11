cmake_minimum_required(VERSION 3.3)
project(virtualpc C)

set(CommonSources
../common/src/array.c)

add_subdirectory(pc)
add_subdirectory(ncas)