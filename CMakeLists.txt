cmake_minimum_required(VERSION 3.15...3.30)

project(
    alloc
    VERSION 0.0.1
    DESCRIPTION "Playing with memory allocators."
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(allocators INTERFACE)
target_include_directories(allocators INTERFACE "${alloc_SOURCE_DIR}/include/")
set_target_properties(allocators PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(app)
