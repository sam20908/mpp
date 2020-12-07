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


# Building with Debug may affect timings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Debug configuration might affect timings!")
endif()

find_package(benchmark CONFIG REQUIRED)

set(PERFORMANCE_TEST_OUTPUT_DIR "${TEST_OUTPUT_DIR}/performance_test")
set(PERFORMANCE_TEST_SUFFIX "test.cpp")

# Executable that contains all performance tests
add_executable(all_tests)

function(_setup_target target)
    target_link_libraries(${target} PRIVATE 
        matrixpp::matrixpp
        benchmark::benchmark benchmark::benchmark_main)

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PERFORMANCE_TEST_OUTPUT_DIR}"

        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
endfunction()

_setup_target(all_tests)

function(_create_performance_test TEST_DIR)
    set(TEST_NAME "${TEST_DIR}")
    set(TEST_SOURCE "${TEST_DIR}/${PERFORMANCE_TEST_SUFFIX}")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    _setup_target(${TEST_NAME})

    target_sources(all_tests PRIVATE "${TEST_SOURCE}")
endfunction()

_create_performance_test("determinant")