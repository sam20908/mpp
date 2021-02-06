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
#include <matrixpp/matrix.hpp>

#include <functional>
#include <stdexcept>
#include <utility>

namespace
{
	// ElementsAre matcher cannot accurately compare irrational values. We check each element manually instead

	TEST(Inverse, DimensionsAndExtents_FullyStatic)
	{
		const auto matrix  = matrixpp::matrix<float, 1, 1>{ 1.F };
		const auto inverse = matrixpp::inverse(matrix);

		EXPECT_EQ(inverse.rows(), 1);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), 1);
		EXPECT_EQ(inverse.columns_extent(), 1);
	}

	TEST(Inverse, DimensionsAndExtents_FullyDynamic)
	{
		const auto matrix  = matrixpp::matrix<float>{ 1, 1, 1.F };
		const auto inverse = matrixpp::inverse(matrix);

		EXPECT_EQ(inverse.rows(), 1);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(inverse.columns_extent(), std::dynamic_extent);
	}

	TEST(Inverse, DimensionsAndExtents_DynamicRows)
	{
		const auto matrix  = matrixpp::matrix<float, std::dynamic_extent, 1>{ 1, 1.F };
		const auto inverse = matrixpp::inverse(matrix);

		EXPECT_EQ(inverse.rows(), 1);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(inverse.columns_extent(), 1);
	}

	TEST(Inverse, DimensionsAndExtents_DynamicColumns)
	{
		const auto matrix  = matrixpp::matrix<float, 1, std::dynamic_extent>{ 1, 1.F };
		const auto inverse = matrixpp::inverse(matrix);

		EXPECT_EQ(inverse.rows(), 1);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), 1);
		EXPECT_EQ(inverse.columns_extent(), std::dynamic_extent);
	}

	TEST(Inverse, NonSquareMatrix_FullyStatic_Throw)
	{
		const auto matrix = matrixpp::matrix<float, 1, 2>{ 1.F };
		EXPECT_THROW((void)(matrixpp::inverse(matrix)), std::runtime_error);
	}

	TEST(Inverse, NonSquareMatrix_FullyDynamic_Throw)
	{
		const auto matrix = matrixpp::matrix<float>{ 1, 2, 1.F };
		EXPECT_THROW((void)(matrixpp::inverse(matrix)), std::runtime_error);
	}

	TEST(Inverse, NonSquareMatrix_DynamicRows_Throw)
	{
		const auto matrix = matrixpp::matrix<float, std::dynamic_extent, 2>{ 1, 1.F };
		EXPECT_THROW((void)(matrixpp::inverse(matrix)), std::runtime_error);
	}


	TEST(Inverse, NonSquareMatrix_DynamicColumns_Throw)
	{
		const auto matrix = matrixpp::matrix<float, 1, std::dynamic_extent>{ 2, 1.F };
		EXPECT_THROW((void)(matrixpp::inverse(matrix)), std::runtime_error);
	}

	TEST(Inverse, 0x0)
	{
		const auto matrix  = matrixpp::matrix<int>{ 0, 0, 0 };
		const auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows(), 0);
		EXPECT_EQ(inverse.columns(), 0);
	}

	TEST(Inverse, 1x1)
	{
		const auto matrix  = matrixpp::matrix<int>{ { 2 } };
		const auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows(), 1);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_FLOAT_EQ(inverse(0, 0), 1.F / 2.F);
	}

	TEST(Inverse, 2x2)
	{
		const auto matrix  = matrixpp::matrix<int>{ { 65, 4 }, { -9, 122 } };
		const auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows(), 2);
		EXPECT_EQ(inverse.columns(), 2);

		EXPECT_FLOAT_EQ(inverse(0, 0), 61.F / 3983.F);
		EXPECT_FLOAT_EQ(inverse(0, 1), -2.F / 3983.F);
		EXPECT_FLOAT_EQ(inverse(1, 0), 9.F / 7966.F);
		EXPECT_FLOAT_EQ(inverse(1, 1), 65.F / 7966.F);
	}

	TEST(Inverse, 3x3)
	{
		const auto matrix  = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		const auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows(), 3);
		EXPECT_EQ(inverse.columns(), 3);

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
		const auto matrix = matrixpp::matrix<int>{ { 2, 4, 6, 9 }, { 0, -1, -8, 1 }, { 0, 0, 96, 4 }, { 0, 0, 96, 5 } };
		const auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows(), 4);
		EXPECT_EQ(inverse.columns(), 4);

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
} // namespace