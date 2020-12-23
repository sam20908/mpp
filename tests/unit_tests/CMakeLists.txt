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

configure_file("lit.site.cfg.py.in" "${UNIT_TESTS_BINARY_DIR}/lit.site.cfg.py")

function(_create_unit_test TEST_DIR)
    set(TEST_NAME "${TEST_DIR}${UNIT_TESTS_EXECUTABLE_SUFFIX}")
    set(TEST_SOURCE "${TEST_DIR}/${UNIT_TESTS_SOURCE_FILENAME}")
    add_executable(${TEST_NAME} 
        "${TEST_SOURCE}" 
        "gtest_main.cpp")

    target_link_libraries(${TEST_NAME} PRIVATE
        matrixpp::matrixpp
        GTest::gmock GTest::gtest)

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${UNIT_TESTS_BINARY_DIR}"
        
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
endfunction()

_create_unit_test("determinant")