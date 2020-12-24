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

find_package(benchmark CONFIG REQUIRED)

set(BENCHMARKS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/benchmarks")
set(BENCHMARKS_SOURCE_FILENAME "test.cpp")
set(BENCHMARKS_LIBRARIES "${PROJECT_NAME}::${PROJECT_NAME} benchmark::benchmark benchmark::benchmark_main")

# This executable will contain all the benchmarks
add_executable("all_benchmarks")
_setup_target("all_benchmarks")

function(_create_performance_test TEST_DIR)
    set(TEST_NAME "${TEST_DIR}_benchmarks")
    set(TEST_SOURCE "${TEST_DIR}/${BENCHMARKS_SOURCE_FILENAME}")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    _setup_target(${TEST_NAME} "${BENCHMARKS_OUTPUT_DIR}" "${BENCHMARKS_LIBRARIES}")

    target_sources("all_benchmarks" PRIVATE "${TEST_SOURCE}")
endfunction()

_create_performance_test("determinant")
_create_performance_test("inverse")