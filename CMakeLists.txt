cmake_minimum_required(VERSION 3.29)

project(markymark LANGUAGES C)

set(CMAKE_C_STANDARD 23)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

list(REMOVE_ITEM MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

add_library(mm_lib STATIC "${MY_SOURCES}" "${MY_HEADERS}")
add_executable(markymark "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

target_include_directories(mm_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries(markymark mm_lib)
target_compile_options(markymark PRIVATE -Wall -Wextra -g)
