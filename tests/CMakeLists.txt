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

set(TESTS_BINARY_DIR "${CMAKE_BINARY_DIR}/bin/tests")

# Copy the LLVM configuration file anyways as it's optional that people use Lit
configure_file("lit.site.cfg.in.py" "${TESTS_BINARY_DIR}/lit.site.cfg.py")

add_custom_target("tests")

function(_create_test TEST_FILENAME)
    set(TEST_NAME "${TEST_FILENAME}_test")
    set(TEST_SOURCE "src/${TEST_FILENAME}.cpp")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    target_include_directories(${TEST_NAME} PRIVATE "thirdparty")

    _setup_target(${TEST_NAME} "${TESTS_BINARY_DIR}")
    _turn_on_warnings(${TEST_NAME})

    add_dependencies("tests" ${TEST_NAME})

    target_compile_definitions(${TEST_NAME} PRIVATE -DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test_data")
endfunction()

# _create_test("algos")
# _create_test("ariths")
# _create_test("assign")
# _create_test("customize")
_create_test("init")
# _create_test("iter")
# _create_test("mem_fns")
# _create_test("utils")

if(${PROJECT_NAME_UPPER}_CODE_COVERAGE)
    include("thirdparty/CodeCoverage.cmake")
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_xml(
        NAME ${PROJECT_NAME}_code_coverage
        EXECUTABLE lit bin/tests # @FIXME: Find a way to workaround failures of tests
        EXCLUDE "${PROJECT_SOURCE_DIR}/tests/*"
        DEPENDENCIES "tests")
endif()
