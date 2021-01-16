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
// RUN: cmake -DTEST_NAME=one_side_zero_and_other_non_zero -DTEST_SOURCE=%s -B build/one_side_zero_and_other_non_zero
// RUN: cmake --build build/one_side_zero_and_other_non_zero --target one_side_zero_and_other_non_zero
// XFAIL: *

#include <matrixpp/matrix.hpp>

int main()
{
	(void)matrixpp::matrix<int, 0, 1>{};

	return 0;
}