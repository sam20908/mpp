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

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <matrixpp/algorithm/transpose.hpp>
#include <matrixpp/matrix.hpp>

#include <span>

namespace
{
	using ::testing::ElementsAre;

	TEST(Transpose, DimensionsAndExtents_FullyStatic)
	{
		const auto matrix  = matrixpp::matrix<int, 1, 2>{ 1 };
		const auto inverse = matrixpp::transpose(matrix);

		EXPECT_EQ(inverse.rows(), 2);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), 2);
		EXPECT_EQ(inverse.columns_extent(), 1);
	}

	TEST(Transpose, DimensionsAndExtents_FullyDynamic)
	{
		const auto matrix  = matrixpp::matrix<int>{ 1, 2, 1 };
		const auto inverse = matrixpp::transpose(matrix);

		EXPECT_EQ(inverse.rows(), 2);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(inverse.columns_extent(), std::dynamic_extent);
	}

	TEST(Transpose, DimensionsAndExtents_DynamicRows)
	{
		const auto matrix  = matrixpp::matrix<int, std::dynamic_extent, 2>{ 1 };
		const auto inverse = matrixpp::transpose(matrix);

		EXPECT_EQ(inverse.rows(), 2);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), 2);
		EXPECT_EQ(inverse.columns_extent(), std::dynamic_extent);
	}

	TEST(Transpose, DimensionsAndExtents_DynamicColumns)
	{
		const auto matrix  = matrixpp::matrix<int, 1, std::dynamic_extent>{ 2, 1 };
		const auto inverse = matrixpp::transpose(matrix);

		EXPECT_EQ(inverse.rows(), 2);
		EXPECT_EQ(inverse.columns(), 1);

		EXPECT_EQ(inverse.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(inverse.columns_extent(), 1);
	}

	TEST(Transpose, 3x2To2x3)
	{
		const auto from = matrixpp::matrix<int>{ { 1, 2 }, { 3, 4 }, { 5, 6 } };
		const auto to   = matrixpp::transpose(from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(1, 3, 5, 2, 4, 6));
	}

	TEST(Transpose, 6x1To1x6)
	{
		const auto from = matrixpp::matrix<int>{ { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 } };
		const auto to   = matrixpp::transpose(from);

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 6);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(1, 2, 3, 4, 5, 6));
	}

	// @TODO: Test all combinations of matrices with extents
} // namespace