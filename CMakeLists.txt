cmake_minimum_required(VERSION 3.20)

project(cinv)

include_directories(include)

file(GLOB_RECURSE SRC_MAIN "src/*.c")

add_executable(cinv ${SRC_MAIN})

set_target_properties(cinv PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")