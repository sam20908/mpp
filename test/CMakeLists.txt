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

set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/test")

function(_map_option_to_subdir OPT SUBDIR HELPER_TEXT)
    # Helper variable to determine whether the test suite should be added:
    # 1. If the current test suite is not explicitly specified by the user, add it
    # 2. If it is explicitly specified, then set it to their specified value (
    #    to handle user setting it to TRUE, which should still add the test suite)
    set(OPT_VAL)
    if(DEFINED ${OPT})
        set(OPT_VAL ${${OPT}})
    else()
        set(OPT_VAL TRUE)
    endif()

    option(${OPT} "${HELPER_TEXT}" ${OPT_VAL})

    if(${${OPT}})
        add_subdirectory(${SUBDIR})
    endif()
endfunction()

_map_option_to_subdir(${PROJECT_NAME_UPPER}_BUILD_COMPILE_TEST "compile_test" "Builds compile test suite")
_map_option_to_subdir(${PROJECT_NAME_UPPER}_BUILD_UNIT_TEST "unit_test" "Builds unit test suite")
_map_option_to_subdir(${PROJECT_NAME_UPPER}_BUILD_PERFORMANCE_TEST "performance_test" "Builds performance test suite")

include(Dart)

add_test(NAME "test" 
    COMMAND lit unit_test compile_test
    WORKING_DIRECTORY "${TEST_OUTPUT_DIR}")