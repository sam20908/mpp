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

find_package(GTest CONFIG REQUIRED)

set(UNIT_TESTS_BINARY_DIR "${TEST_OUTPUT_DIR}/unit_tests")
set(UNIT_TESTS_SOURCE_FILENAME "test.cpp")
set(UNIT_TESTS_EXECUTABLE_SUFFIX "_unit_test")
set(UNIT_TESTS_LIBRARIES ${PROJECT_NAME}::${PROJECT_NAME} GTest::gmock GTest::gtest)

configure_file("lit.site.cfg.py.in" "${UNIT_TESTS_BINARY_DIR}/lit.site.cfg.py")

# This executable will contain all the unit tests
add_executable("all_unit_tests")
target_sources("all_unit_tests" PRIVATE "gtest_main.cpp")
_setup_target("all_unit_tests" "${UNIT_TESTS_BINARY_DIR}" "${UNIT_TESTS_LIBRARIES}")

function(_create_unit_test TEST_DIR)
    set(TEST_NAME "${TEST_DIR}${UNIT_TESTS_EXECUTABLE_SUFFIX}")
    set(TEST_SOURCE "${TEST_DIR}/${UNIT_TESTS_SOURCE_FILENAME}")
    add_executable(${TEST_NAME} 
        "${TEST_SOURCE}" 
        "gtest_main.cpp")

    _setup_target(${TEST_NAME} "${UNIT_TESTS_BINARY_DIR}" "${UNIT_TESTS_LIBRARIES}")

    target_sources("all_unit_tests" PRIVATE "${TEST_SOURCE}")
endfunction()

_create_unit_test("determinant")

cmake_dependent_option(${PROJECT_NAME_UPPER}_CODE_COVERAGE "Enables code coverage" 
    ON "${PROJECT_NAME_UPPER}_BUILD_TESTS" OFF)

if(${PROJECT_NAME_UPPER}_CODE_COVERAGE)
    include("CodeCoverage.cmake")
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_xml(
        NAME ${PROJECT_NAME}_code_coverage
        EXECUTABLE "bin/tests/unit_tests/${CMAKE_BUILD_TYPE}/all_unit_tests"
        EXCLUDES "vcpkg/*"
        DEPENDENCIES "all_unit_tests")
endif()