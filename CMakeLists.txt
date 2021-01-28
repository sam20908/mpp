# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at

#   http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

function(_setup_target target binary_dir libraries)
    target_link_libraries(${target} PRIVATE ${libraries})

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${binary_dir}"

        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
endfunction()

function(_turn_on_warnings target)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion
            -Wsign-conversion -Wnon-virtual-dtor -Werror -pedantic-errors)
    else()
        target_compile_options(${target} PRIVATE /W4 /WX)
    endif()
endfunction()

cmake_minimum_required(VERSION 3.17)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project("matrixpp")
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} INTERFACE
    # Main headers
    "include/matrixpp/algorithm.hpp"
    "include/matrixpp/operation.hpp"
    "include/matrixpp/matrix.hpp"
    "include/matrixpp/utility.hpp"

    # Matrix headers
    "include/matrixpp/matrix/dynamic_rows.hpp"
    "include/matrixpp/matrix/dynamic_columns.hpp"
    "include/matrixpp/matrix/fully_dynamic.hpp"
    "include/matrixpp/matrix/fully_static.hpp"

    # Algorithm headers
    "include/matrixpp/algorithm/block.hpp"
    "include/matrixpp/algorithm/determinant.hpp"
    "include/matrixpp/algorithm/inverse.hpp"
    "include/matrixpp/algorithm/transpose.hpp"
    
    # Operation headers
    "include/matrixpp/operation/add.hpp"
    "include/matrixpp/operation/subtract.hpp"
    "include/matrixpp/operation/multiply.hpp"
    "include/matrixpp/operation/divide.hpp"

    # Utility headers
    "include/matrixpp/utility/cast.hpp"
    "include/matrixpp/utility/config.hpp"
    "include/matrixpp/utility/equal.hpp"
    "include/matrixpp/utility/square.hpp"
    "include/matrixpp/utility/type.hpp"

    # Detail headers
    "include/matrixpp/detail/algo_types.hpp"
    "include/matrixpp/detail/constraints.hpp"
    "include/matrixpp/detail/expr_base.hpp"
    "include/matrixpp/detail/expr_binary_constant_op.hpp"
    "include/matrixpp/detail/expr_binary_op.hpp"
    "include/matrixpp/detail/matrix_base.hpp"
    "include/matrixpp/detail/tag_invoke.hpp"
    "include/matrixpp/detail/utility.hpp")

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

option(${PROJECT_NAME_UPPER}_BUILD_TESTS "Builds the test suites" OFF)
option(${PROJECT_NAME_UPPER}_BUILD_BENCHMARKS "Builds the benchmarks" OFF)

if(${PROJECT_NAME_UPPER}_BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()

if(${PROJECT_NAME_UPPER}_BUILD_BENCHMARKS)
    add_subdirectory("benchmarks")
endif()