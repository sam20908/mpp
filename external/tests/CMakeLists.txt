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
set(TESTS_LIBRARIES ${PROJECT_NAME}::${PROJECT_NAME})

if(LLVM_LIT_EXEC)
    message(STATUS "LLVM Lit found! LLVM Lit configuration for tests will be created")
    configure_file("lit.site.cfg.in.py" "${TESTS_BINARY_DIR}/lit.site.cfg.py")
else()
    message(STATUS "LLVM Lit not found! LLVM Lit configuration for tests will not be created")
endif()

add_custom_target("tests")

function(_create_test TEST_FILENAME)
    set(TEST_NAME "${TEST_FILENAME}_test")
    set(TEST_SOURCE "src/${TEST_FILENAME}.cpp")
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    _setup_target(${TEST_NAME} "${TESTS_BINARY_DIR}")
    _turn_on_warnings(${TEST_NAME})

    add_dependencies("tests" ${TEST_NAME})

    target_compile_definitions(${TEST_NAME} PRIVATE -DTEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()

_create_test("customization")
_create_test("utility_cpos")
_create_test("iterator")
_create_test("algorithm_cpos")

if(LLVM_LIT_EXEC)
    add_test(NAME "tests" 
        COMMAND ${LLVM_LIT_EXEC} .
        WORKING_DIRECTORY "${TESTS_BINARY_DIR}")
endif()
