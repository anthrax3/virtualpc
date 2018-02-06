cmake_minimum_required(VERSION 3.3)
project(virtualpc C)

set(CMAKE_C_FLAGS "-Werror -Wall -std=c89")

file(GLOB_RECURSE SOURCES "src/*.c") 

add_executable(pc ${SOURCES})
target_compile_definitions(pc PRIVATE _POSIX_C_SOURCE=200112L)
include_directories(include) 