/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=non_arithmetic_type -DTEST_SOURCE=%s -B build/non_arithmetic_type
// RUN: cmake --build build/non_arithmetic_type --target non_arithmetic_type
// XFAIL: *

#include <matrixpp/matrix.hpp>

struct non_arithmetic_type
{
};

int main()
{
	(void)matrixpp::matrix<non_arithmetic_type>{};

	return 0;
}