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

#include <gtest/gtest.h>

#include <mpp/matrix/fully_dynamic.hpp>
#include <mpp/utility/singular.hpp>

#include <utility>

namespace
{
	TEST(Singular, 1x1_0_Initialized)
	{
		const auto matrix      = mpp::matrix<int>{ 1, 1, 0 };
		const auto is_singular = mpp::singular(matrix);

		EXPECT_TRUE(is_singular);
	}

	TEST(Singular, 1x1)
	{
		const auto matrix      = mpp::matrix<int>{ { 123456 } };
		const auto is_singular = mpp::singular(matrix);

		EXPECT_FALSE(is_singular);
	}

	TEST(Singular, 2x2)
	{
		const auto matrix      = mpp::matrix<float>{ { 123.F, 4234.F }, { 1.F / 2.F, -7983.F } };
		const auto is_singular = mpp::singular(matrix);

		EXPECT_FALSE(is_singular);
	}


	TEST(Singular, 3x3)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		const auto is_singular = mpp::singular(matrix);

		EXPECT_FALSE(is_singular);
	}

	TEST(Singular, 6x6)
	{
		const auto matrix      = mpp::matrix<int>{ { 65, 4, 5, 3, 3, 4 },
            { -9, 122, -234, 6, 5, 23 },
            { 7, 3, 5, 32, 5, 4 },
            { 4, 2, 2, 4, 2, 4 },
            { 5, 34, 1, 45, -12, 4 },
            { 1, 2, 23, 6, 543, 4 } };
		const auto is_singular = mpp::singular(matrix);

		EXPECT_FALSE(is_singular);
	}
} // namespace