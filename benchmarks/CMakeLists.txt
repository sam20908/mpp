
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

set(BENCHMARKS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/benchmarks")

cmake_dependent_option(${PROJECT_NAME_UPPER}_BUILD_COMPILE_TIME_BENCHMARKS "Builds compile time benchmarks suite" 
    ON "${PROJECT_NAME_UPPER}_BUILD_BENCHMARKS" OFF)
cmake_dependent_option(${PROJECT_NAME_UPPER}_BUILD_RUNTIME_BENCHMARKS "Builds runtime benchmarks suite" 
    ON "${PROJECT_NAME_UPPER}_BUILD_BENCHMARKS" OFF)

if(${PROJECT_NAME_UPPER}_BUILD_COMPILE_TIME_BENCHMARKS)
    # @TODO: Enable this after added compile benchmarks
    # add_subdirectory("compile_time_benchmarks")
endif()

if(${PROJECT_NAME_UPPER}_BUILD_RUNTIME_BENCHMARKS)
    add_subdirectory("runtime_benchmarks")
endif()