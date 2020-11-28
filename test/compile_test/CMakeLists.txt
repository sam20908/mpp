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

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
set(COMPILE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(COMPILE_TEST_OUTPUT_DIR "${TEST_OUTPUT_DIR}/compile_test")
set(COMPILE_TEST_SUFFIX "test.cpp")

configure_file("lit.site.cfg.py.in" "${COMPILE_TEST_OUTPUT_DIR}/lit.site.cfg.py")
configure_file("CMakeLists.txt.in" "${COMPILE_TEST_OUTPUT_DIR}/CMakeLists.txt" @ONLY)