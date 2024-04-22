cmake_minimum_required(VERSION 3.20)

project(cinv)

include_directories(include)

add_subdirectory(3rdparty/raylib)
add_subdirectory(3rdparty/raylib-nuklear)

file(GLOB_RECURSE SRC_MAIN "src/*.c")

add_executable(cinv ${SRC_MAIN})

set_target_properties(cinv PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

target_link_libraries(cinv raylib raylib_nuklear)