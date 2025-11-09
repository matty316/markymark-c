cmake_minimum_required(VERSION 3.29)

project(markymark LANGUAGES C)

set(CMAKE_C_STANDARD 23)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(markymark "${MY_SOURCES}" "${MY_HEADERS}")
target_include_directories(markymark PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_compile_options(markymark PRIVATE -Wall -Wextra -g)
