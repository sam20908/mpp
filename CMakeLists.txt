cmake_minimum_required(VERSION 3.16)

project("matrixpp")

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(matrixpp INTERFACE "${CMAKE_SOURCE_DIR}/include/matrixpp/matrixpp.hpp")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wnon-virtual-dtor -Werror -pedantic-errors)
else()
    target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX)
endif()