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

include(CMakeDependentOption)
include(Dart)

set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/test")

cmake_dependent_option(${PROJECT_NAME_UPPER}_BUILD_COMPILE_TEST "Builds compile test suite" 
    ON "${PROJECT_NAME_UPPER}_BUILD_TEST" OFF)
cmake_dependent_option(${PROJECT_NAME_UPPER}_BUILD_UNIT_TEST "Builds unit test suite" 
    ON "${PROJECT_NAME_UPPER}_BUILD_TEST" OFF)
cmake_dependent_option(${PROJECT_NAME_UPPER}_BUILD_PERFORMANCE_TEST "Builds performance test suite" 
    ON "${PROJECT_NAME_UPPER}_BUILD_TEST" OFF)

if(${PROJECT_NAME_UPPER}_BUILD_COMPILE_TEST)
    add_subdirectory("compile_test")
endif()

if(${PROJECT_NAME_UPPER}_BUILD_UNIT_TEST)
    add_subdirectory("unit_test")
endif()

if(${PROJECT_NAME_UPPER}_BUILD_PERFORMANCE_TEST)
    add_subdirectory("performance_test")
endif()

add_test(NAME "test" 
    COMMAND lit unit_test compile_test
    WORKING_DIRECTORY "${TEST_OUTPUT_DIR}")