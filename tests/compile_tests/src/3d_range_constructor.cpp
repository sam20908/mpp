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
// RUN: cmake -DTEST_NAME=3d_range_constructor -DTEST_SOURCE=%s -B build/3d_range_constructor
// RUN: cmake --build build/3d_range_constructor --target 3d_range_constructor
// XFAIL: *

#include <array>
#include <matrixpp/matrix.hpp>

int main()
{
	std::array<std::array<std::array<int, 1>, 1>, 1> rng_3d{ std::array{ std::array{ 1 } } };
	(void)matrixpp::matrix<int>{ rng_3d };

	return 0;
}