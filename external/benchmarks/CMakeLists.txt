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

set(BENCHMARKS_BINARY_DIR "${CMAKE_BINARY_DIR}/bin/benchmarks")

add_custom_target("benchmarks")

function(_create_benchmark TEST_FILE)
    set(TEST_NAME "${TEST_FILE}_benchmark")
    set(TEST_SOURCE "src/${TEST_FILE}.cpp")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    _setup_target(${TEST_NAME} "${BENCHMARKS_BINARY_DIR}")
    _turn_on_warnings(${TEST_NAME})

    add_dependencies("benchmarks" ${TEST_NAME})
endfunction()

_create_benchmark("initialization")
