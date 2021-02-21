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

#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/dummy_expr.hpp"

#include <array>
#include <span>
#include <utility>
#include <vector>

namespace
{
	using ::testing::ElementsAre;

	/**
	 * Fully static matrices
	 */

	TEST(Assignment, FullyStaticCopyAssignment)
	{
		const auto dummy = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix      = mpp::matrix<int, 2, 3>{};

		matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyStaticMoveAssignment)
	{
		auto dummy  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, 2, 3>{};

		matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyStatic2DInitializerList)
	{
		auto init_2d = std::initializer_list<std::initializer_list<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix  = mpp::matrix<int, 2, 3>{};

		matrix = init_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyStatic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix       = mpp::matrix<int, 2, 3>{};

		matrix = rng_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyStatic2DRange_Move)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, 2, 3>{};

		matrix = std::move(rng_2d);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	/**
	 * Fully dynamic matrices
	 */

	TEST(Assignment, FullyDynamicCopyAssignment)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix      = mpp::matrix<int>{};

		matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyDynamicMoveAssignment)
	{
		auto dummy  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int>{};

		matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyDynamic2DInitializerList)
	{
		auto init_2d = std::initializer_list<std::initializer_list<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix  = mpp::matrix<int>{ 2, 3 };

		matrix = init_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyDynamic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix       = mpp::matrix<int>{ 2, 3 };

		matrix = rng_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, FullyDynamic2DRange_Move)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int>{ 2, 3 };

		matrix = std::move(rng_2d);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	/**
	 * Dynamic rows matrices
	 */

	TEST(Assignment, DynamicRowsCopyAssignment)
	{
		const auto dummy = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{};

		matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicRowsMoveAssignment)
	{
		auto dummy  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{};

		matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicRows2DInitializerList)
	{
		auto init_2d = std::initializer_list<std::initializer_list<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		matrix = init_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicRows2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix       = mpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		matrix = rng_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicRows2DRange_Move)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		matrix = std::move(rng_2d);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	/**
	 * Dynamic columns matrices
	 */

	TEST(Assignment, DynamicColumnsCopyAssignment)
	{
		const auto dummy = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{};

		matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicColumnsMoveAssignment)
	{
		auto dummy  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{};

		matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicColumns2DInitializerList)
	{
		auto init_2d = std::initializer_list<std::initializer_list<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ 3 };

		matrix = init_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicColumns2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix       = mpp::matrix<int, 2, std::dynamic_extent>{ 3 };

		matrix = rng_2d;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Assignment, DynamicColumns2DRange_Move)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ 3 };

		matrix = std::move(rng_2d);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}
} // namespace