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

if(NOT LLVM_LIT_EXEC)
    message(FATAL_ERROR "Compile tests requires LLVM Lit to be found in PATH!")
else()
    message(STATUS "LLVM Lit found! LLVM Lit configuration for compile tests will be created")
endif()

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set(COMPILE_TESTS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(COMPILE_TESTS_BINARY_DIR "${TEST_OUTPUT_DIR}/compile_tests")

set(ADDED_COMPILE_TESTS)

function(_create_compile_test TEST_NAME)
    add_custom_target("${TEST_NAME}_compile_test"
        COMMAND ${LLVM_LIT_EXEC} --filter ${TEST_NAME} .
        WORKING_DIRECTORY "${COMPILE_TESTS_BINARY_DIR}")

    set(ADDED_COMPILE_TESTS "${ADDED_COMPILE_TESTS}${TEST_NAME}\|" PARENT_SCOPE)
endfunction()

configure_file("lit.site.cfg.in.py" "${COMPILE_TESTS_BINARY_DIR}/lit.site.cfg.py")
configure_file("CMakeLists.txt.in" "${COMPILE_TESTS_BINARY_DIR}/CMakeLists.txt" @ONLY)

_create_compile_test("deduction_guides")
_create_compile_test("rule_of_five")
_create_compile_test("iterator_requirements")
_create_compile_test("container_requirements")
_create_compile_test("allocator_aware_container")

# These below so that after the compile tests are added, the filter string is formed

string(REGEX REPLACE "\\|$" "" ADDED_COMPILE_TESTS_PROPER ${ADDED_COMPILE_TESTS})

if(LLVM_LIT_EXEC)
    add_test(NAME "compile_tests" 
        COMMAND ${LLVM_LIT_EXEC} . --filter=${ADDED_COMPILE_TESTS_PROPER}
        WORKING_DIRECTORY "${COMPILE_TESTS_BINARY_DIR}")
endif()

add_custom_target("compile_tests"
    COMMAND ctest -R compile_tests -V
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")