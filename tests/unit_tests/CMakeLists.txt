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
set(UNIT_TESTS_EXECUTABLE_SUFFIX "unit_test")
set(UNIT_TESTS_LIBRARIES ${PROJECT_NAME}::${PROJECT_NAME} GTest::gmock GTest::gtest)

set(INCLUDE_HEADERS "../include/dummy_expr.hpp")

if(LLVM_LIT_EXEC)
    message(STATUS "LLVM Lit found! LLVM Lit configuration for unit tests will be created")
    configure_file("lit.site.cfg.py.in" "${UNIT_TESTS_BINARY_DIR}/lit.site.cfg.py")
else()
    message(STATUS "LLVM Lit not found! LLVM Lit configuration for unit tests will not be created")
endif()

add_custom_target("unit_test")

function(_create_unit_test TEST_FILENAME)
    set(TEST_NAME "${TEST_FILENAME}_${UNIT_TESTS_EXECUTABLE_SUFFIX}")
    set(TEST_SOURCE "src/${TEST_FILENAME}.cpp")
    add_executable(${TEST_NAME} 
        "${TEST_SOURCE}" 
        "${INCLUDE_HEADERS}"
        "gtest_main.cpp")

    _setup_target(${TEST_NAME} "${UNIT_TESTS_BINARY_DIR}" "${UNIT_TESTS_LIBRARIES}")
    _turn_on_warnings(${TEST_NAME})

    add_dependencies("unit_test" ${TEST_NAME})
endfunction()

cmake_dependent_option(${PROJECT_NAME_UPPER}_CODE_COVERAGE "Enables code coverage" 
    OFF "${PROJECT_NAME_UPPER}_BUILD_TESTS" OFF)

if(${PROJECT_NAME_UPPER}_CODE_COVERAGE)
    include("CodeCoverage.cmake")
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_xml(
        NAME ${PROJECT_NAME}_code_coverage
        EXECUTABLE "ctest"
        EXCLUDE "${PROJECT_SOURCE_DIR}/vcpkg/*" "${PROJECT_SOURCE_DIR}/tests/*"
        DEPENDENCIES "unit_test")
endif()

if(LLVM_LIT_EXEC)
    add_test(NAME "unit_test" 
        COMMAND ${LLVM_LIT_EXEC} .
        WORKING_DIRECTORY "${UNIT_TESTS_BINARY_DIR}")
endif()

# Algorithm tests
_create_unit_test("block")
_create_unit_test("determinant")
_create_unit_test("inverse")
_create_unit_test("transpose")

# Utility tests
_create_unit_test("cast")
_create_unit_test("singular")
_create_unit_test("type")

# Basic tests
_create_unit_test("customization")
_create_unit_test("initialization")
_create_unit_test("arithmetic")