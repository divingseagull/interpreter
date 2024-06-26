cmake_minimum_required(VERSION 3.25)
project(interpreter_cmake C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
add_library(hashmap STATIC include/hashmap.h
    include/hashmap.c)
add_library(cvector STATIC include/cvector.h
    include/cvector.c)
add_executable(interpreter_cmake src/main.c)
target_link_libraries(interpreter_cmake hashmap cvector)

