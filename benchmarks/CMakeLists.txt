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

set(BENCHMARKS_BINARY_DIR "${CMAKE_BINARY_DIR}/bin/benchmarks")
set(BENCHMARKS_LIBRARIES ${PROJECT_NAME}::${PROJECT_NAME} benchmark::benchmark benchmark::benchmark_main)

if(LLVM_LIT_EXEC)
    message(STATUS "LLVM Lit found! LLVM Lit configuration for benchmarks will be created")
    configure_file("lit.site.cfg.in.py" "${BENCHMARKS_BINARY_DIR}/lit.site.cfg.py")
    configure_file("format.in.py" "${BENCHMARKS_BINARY_DIR}/format.py")
else()
    message(STATUS "LLVM Lit not found! LLVM Lit configuration for benchmarks will not be created")
endif()

add_custom_target("benchmark")

function(_create_benchmark TEST_FILE)
    set(TEST_NAME "${TEST_FILE}_benchmark")
    set(TEST_SOURCE "src/${TEST_FILE}.cpp")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    _setup_target(${TEST_NAME} "${BENCHMARKS_BINARY_DIR}" "${BENCHMARKS_LIBRARIES}")
    _turn_on_warnings(${TEST_NAME})

    add_dependencies("benchmark" ${TEST_NAME})
endfunction()

if(LLVM_LIT_EXEC)
    add_test(NAME "benchmark" 
        COMMAND ${LLVM_LIT_EXEC} .
        WORKING_DIRECTORY "${BENCHMARKS_BINARY_DIR}")
endif()

_create_benchmark("algorithms")
_create_benchmark("arithmetic")
_create_benchmark("initialization")
_create_benchmark("utilities")