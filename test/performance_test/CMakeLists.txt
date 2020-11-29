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

function(_create_performance_test TEST_DIR)
    set(TEST_NAME "${TEST_DIR}")
    add_executable(${TEST_NAME} "${TEST_DIR}/${PERFORMANCE_TEST_SUFFIX}")

    target_link_libraries(${TEST_NAME} PRIVATE 
        matrixpp::matrixpp
        benchmark::benchmark benchmark::benchmark_main)
        
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PERFORMANCE_TEST_OUTPUT_DIR}"
        
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
endfunction()

_create_performance_test("determinant")