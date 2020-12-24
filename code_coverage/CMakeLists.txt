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

set(CODE_COVERAGE_BINARY_DIR "${CMAKE_BINARY_DIR}/bin/code_coverage")

add_executable("${PROJECT_NAME}_dummy" "dummy_main.cpp")
target_link_libraries("${PROJECT_NAME}_dummy" ${PROJECT_NAME}::${PROJECT_NAME})
set_target_properties("${PROJECT_NAME}_dummy" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CODE_COVERAGE_BINARY_DIR}"

    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

include("CodeCoverage.cmake")
append_coverage_compiler_flags()
setup_target_for_coverage_gcovr_xml(
    NAME ${PROJECT_NAME}_code_coverage
    EXECUTABLE "bin/code_coverage/Debug/${PROJECT_NAME}_dummy"
    EXCLUDE "code_coverage/*" # Don't include dummy main file
    DEPENDENCIES "${PROJECT_NAME}_dummy")