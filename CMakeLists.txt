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

cmake_minimum_required(VERSION 3.15)

project(mpp CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

function(_setup_target target binary_dir)
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
    target_compile_definitions(${target} PRIVATE -DBOOST_UT_DISABLE_MODULE)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE /bigobj)
    endif()

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${binary_dir}"

        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
endfunction()

function(_turn_on_warnings target)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion
            -Wsign-conversion -Wnon-virtual-dtor -Werror -pedantic-errors)
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion
            -Wsign-conversion -Wnon-virtual-dtor -Werror)
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC)
        # Clang-cl's /W4 is the same as -Wall -Wextra
        # -Wall in Clang-cl turns into -Weverything because it's emulating
        # MSVC's CLI, but we can specify MSVC's /W4 command to workaround this issue
        target_compile_options(${target} PRIVATE /W4 -Wpedantic -Wconversion
            -Wsign-conversion -Wnon-virtual-dtor -Werror)
    endif()

    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
    endif()
endfunction()

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

file(GLOB_RECURSE INCLUDE_HEADERS CONFIGURE_DEPENDS "include/*")
target_sources(${PROJECT_NAME} INTERFACE ${INCLUDE_HEADERS})

# @TODO: Don't make this required due to #172
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE Threads::Threads)

if(${PROJECT_NAME_UPPER}_BUILD_TESTS)
    add_subdirectory("tests")
endif()
