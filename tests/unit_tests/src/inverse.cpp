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
#include <matrixpp/algorithm/inverse.hpp>
#include <matrixpp/matrix/fully_dynamic.hpp>
#include <utility>

namespace
{
	TEST(Inverse, 3x3)
	{
		auto matrix  = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_FLOAT_EQ(inverse(0, 0), 0.F);
		EXPECT_FLOAT_EQ(inverse(0, 1), 1.F / 3.F);
		EXPECT_FLOAT_EQ(inverse(0, 2), -1.F / 3.F);
		EXPECT_FLOAT_EQ(inverse(1, 0), -1.F);
		EXPECT_FLOAT_EQ(inverse(1, 1), 3.F / 2.F);
		EXPECT_FLOAT_EQ(inverse(1, 2), -1.F);
		EXPECT_FLOAT_EQ(inverse(2, 0), 4.F);
		EXPECT_FLOAT_EQ(inverse(2, 1), -41.F / 6.F);
		EXPECT_FLOAT_EQ(inverse(2, 2), 16.F / 3.F);
	}

	TEST(Inverse, 4x4)
	{
		auto matrix  = matrixpp::matrix{ { { 2, 4, 6, 9 }, { 0, -1, -8, 1 }, { 0, 0, 96, 4 }, { 0, 0, 96, 5 } } };
		auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_FLOAT_EQ(inverse(0, 0), 1.F / 2.F);
		EXPECT_FLOAT_EQ(inverse(0, 1), 2.F);
		EXPECT_FLOAT_EQ(inverse(0, 2), 689.F / 96.F);
		EXPECT_FLOAT_EQ(inverse(0, 3), -169.F / 24.F);
		EXPECT_FLOAT_EQ(inverse(1, 0), 0.F);
		EXPECT_FLOAT_EQ(inverse(1, 1), -1.F);
		EXPECT_FLOAT_EQ(inverse(1, 2), -17.F / 12.F);
		EXPECT_FLOAT_EQ(inverse(1, 3), 4.F / 3.F);
		EXPECT_FLOAT_EQ(inverse(2, 0), 0.F);
		EXPECT_FLOAT_EQ(inverse(2, 1), 0.F);
		EXPECT_FLOAT_EQ(inverse(2, 2), 5.F / 96.F);
		EXPECT_FLOAT_EQ(inverse(2, 3), -1.F / 24.F);
		EXPECT_FLOAT_EQ(inverse(3, 0), 0.F);
		EXPECT_FLOAT_EQ(inverse(3, 1), 0.F);
		EXPECT_FLOAT_EQ(inverse(3, 2), -1.F);
		EXPECT_FLOAT_EQ(inverse(3, 3), 1.F);
	}

	// TEST(Inverse, 5x5)
	// {
	// 	// auto matrix = matrixpp::matrix<int, 3, 3>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
	// 	auto matrix = matrixpp::matrix<int, 5, 5>{
	// 		{ { 2, 4, 6, 9, 1 }, { 0, -1, -8, 1, -2 }, { 0, 0, 96, 4, 3 }, { 0, 0, 96, 5, 5 }, { 0, 0, 96, 5, 0 } }
	// 	};
	// 	auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);
	// 	(void)inverse;

	// 	// EXPECT_FLOAT_EQ(inverse(0, 0), 0.F);
	// 	// EXPECT_FLOAT_EQ(inverse(0, 1), 1.F / 3.F);
	// 	// EXPECT_FLOAT_EQ(inverse(0, 2), -1.F / 3.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 0), -1.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 1), 3.F / 2.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 2), -1.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 0), 4.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 1), -41.F / 6.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 2), 16.F / 3.F);
	// }

	// TEST(Inverse, 9x9)
	// {
	// 	// auto matrix  = matrixpp::matrix<int, 3, 3>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
	// 	// @TODO: Fixme inv(U) should be https://gyazo.com/24d40f278cacb140573821d79c9f0748
	// 	/*
	// 	1 9 8 2 3 4 5 7 6
	// 	2 8 5 6 9 1 2 0 4
	// 	1 6 5 8 2 7 3 4 5
	// 	9 8 7 6 0 9 5 6 4
	// 	1 2 3 4 2 9 4 5 6
	// 	0 8 9 6 7 8 1 2 3
	// 	8 9 0 1 6 7 8 5 6
	// 	9 0 6 8 8 9 6 7 8
	// 	1 2 4 2 1 2 3 1 4
	// 	 */
	// 	// auto matrix  = matrixpp::matrix<int, 9, 9>{ { { 1, 9, 8, 2, 3, 4, 5, 7, 6 },
	// 	//     { 2, 8, 5, 6, 9, 1, 2, 0, 4 },
	// 	//     { 1, 6, 5, 8, 2, 7, 3, 4, 5 },
	// 	//     { 9, 8, 7, 6, 0, 9, 5, 6, 4 },
	// 	//     { 1, 2, 3, 4, 2, 9, 4, 5, 6 },
	// 	//     { 0, 8, 9, 6, 7, 8, 1, 2, 3 },
	// 	//     { 8, 9, 0, 1, 6, 7, 8, 5, 6 },
	// 	//     { 9, 0, 6, 8, 8, 9, 6, 7, 8 },
	// 	//     { 1, 2, 4, 2, 1, 2, 3, 1, 4 } } };
	// 	// auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);
	// 	// (void)inverse;

	// 	// EXPECT_FLOAT_EQ(inverse(0, 0), 0.F);
	// 	// EXPECT_FLOAT_EQ(inverse(0, 1), 1.F / 3.F);
	// 	// EXPECT_FLOAT_EQ(inverse(0, 2), -1.F / 3.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 0), -1.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 1), 3.F / 2.F);
	// 	// EXPECT_FLOAT_EQ(inverse(1, 2), -1.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 0), 4.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 1), -41.F / 6.F);
	// 	// EXPECT_FLOAT_EQ(inverse(2, 2), 16.F / 3.F);
	// }

	// TEST(Inverse, 0x0)
	// {
	// 	auto matrix  = matrixpp::matrix<int, 0, 0>{};
	// 	auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

	// 	EXPECT_EQ(inverse.rows(), 0);
	// 	EXPECT_EQ(inverse.columns(), 0);
	// }
} // namespace