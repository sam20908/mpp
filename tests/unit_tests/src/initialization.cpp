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
#include <stdexcept>
#include <utility>
#include <vector>

namespace
{
	using ::testing::ElementsAre;

	/**
	 * Fully static matrices
	 */

	TEST(Initialization, FullyStaticCopyConstruct)
	{
		const auto dummy  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStaticMoveConstruct)
	{
		auto dummy        = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

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

	TEST(Initialization, FullyStatic2DInitializerList)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DInitializerList_SizeMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 2, 3>{ { 7, 3 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, FullyStatic2DInitializerList_InitializerColumnsMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, FullyStatic2DRange_Range2DColumnsMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8 } };
		EXPECT_THROW((void)(mpp::matrix<int, 2, 3>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, FullyStatic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DRange_Copy_SizeMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3 }, { 8, 8 } };

		EXPECT_THROW((void)(mpp::matrix<int, 2, 3>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, FullyStatic2DRange_Move)
	{
		auto rng_2d       = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, 3>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DRange_Move_SizeMismatch_Throw)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3 }, { 8, 8 } };

		EXPECT_THROW((void)(mpp::matrix<int, 2, 3>{ std::move(rng_2d) }), std::invalid_argument);
	}

	TEST(Initialization, FullyStatic2DArray_Copy)
	{
		const auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, 3>{ arr_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DArray_Move)
	{
		auto arr_2d       = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, 3>{ std::move(arr_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStaticCallable)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto matrix = mpp::matrix<int, 2, 3>{ iota };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	// Can't test array size mismatch for fully static matrices because that results in no viable overload, thus compile
	// error

	TEST(Initialization, FullyStatic_StaticExprObject)
	{
		const auto dummy = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		const auto expr   = mpp_test::dummy_expr{ dummy };
		const auto matrix = mpp::matrix<int, 2, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic_DynamicExprObject_SameSize)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		const auto matrix = mpp::matrix<int, 2, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic_DynamicExprObject_DifferentSize_Throw)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		EXPECT_THROW((mpp::matrix<int, 3, 3>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, FullyStaticDefaultConstructor)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, FullyStaticValueConstructor)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{ 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, FullyStaticIdentityMatrix)
	{
		const auto matrix = mpp::matrix<int, 3, 3>{ mpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, FullyStaticIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 3, 4>{ mpp::identity_matrix }), std::invalid_argument);
	}

	TEST(Initialization, FullyStaticIdentityMatrix_0x0_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 0, 0>{ mpp::identity_matrix }), std::invalid_argument);
	}

	/**
	 * Fully dynamic matrices
	 */

	TEST(Initialization, FullyDynamicCopyConstruct)
	{
		const auto dummy  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamicCopyConstruct_CustomAllocator)
	{
		const auto dummy =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ { 7, 3, 1 },
				{ 8, 8, 2 } };
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ dummy,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamicMoveConstruct)
	{
		auto dummy        = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamicMoveConstruct_CustomAllocator)
	{
		auto dummy =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ { 7, 3, 1 },
				{ 8, 8, 2 } };
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{
			std::move(dummy),
			allocator
		};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

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

	TEST(Initialization, FullyDynamic2DInitializerList)
	{
		const auto matrix = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DInitializerList_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{
			{ { 7, 3, 1 }, { 8, 8, 2 } },
			allocator
		};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DInitializerList_InitializerColumnsMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, FullyDynamic2DRange_Range2DColumnsMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8 } };
		EXPECT_THROW((void)(mpp::matrix<int>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, FullyDynamic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DRange_Copy_CustomAllocator)
	{
		const auto rng_2d    = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ rng_2d,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DRange_Move)
	{
		auto rng_2d       = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DRange_Move_CustomAllocator)
	{
		auto rng_2d          = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{
			std::move(rng_2d),
			allocator
		};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	// Can't test size mismatch of 2D ranges for fully dynamic matrices because fully dynamic matrices accept any size

	TEST(Initialization, FullyDynamic_ExprObject)
	{
		// Since fully dynamic matrices take any sized expression object, it is not possible for them to throw an
		// exception, therefore no need to write a separate test for that

		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		const auto matrix = mpp::matrix<int>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic_ExprObject_CustomAllocator)
	{
		const auto dummy     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto expr      = mpp_test::dummy_expr{ dummy };

		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ expr,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamicDefaultConstructor)
	{
		const auto matrix = mpp::matrix<int>{};

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);
	}

	TEST(Initialization, FullyDynamicDefaultConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{};

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, FullyDynamicConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ allocator };

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, FullyDynamicSizeConstructor)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{ 2, 3 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, FullyDynamicSizeConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 2,
				3,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, FullyDynamicValueConstructor)
	{
		const auto matrix = mpp::matrix<int>{ 2, 3, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, FullyDynamicValueConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 2,
				3,
				1,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, FullyDynamicIdentityMatrix)
	{
		const auto matrix = mpp::matrix<int>{ 3, 3, mpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, FullyDynamicIdentityMatrix_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 3,
				3,
				mpp::identity_matrix,
				allocator };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, FullyDynamicCallable)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto matrix = mpp::matrix<int>{ 2, 3, iota };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, FullyDynamicCallable_CustomAllocator)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 2,
				3,
				iota,
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, FullyDynamicIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int>{ 3, 4, mpp::identity_matrix }), std::invalid_argument);
	}

	TEST(Initialization, FullyDynamicIdentityMatrix_0x0_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int>{ 0, 0, mpp::identity_matrix }), std::invalid_argument);
	}

	/**
	 * Dynamic rows matrices
	 */

	TEST(Initialization, DynamicRowsCopyConstruct)
	{
		const auto dummy  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRowsCopyConstruct_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto dummy     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ dummy, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRowsMoveConstruct)
	{
		auto dummy        = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRowsMoveConstruct_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		auto dummy           = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ std::move(dummy), allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

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

	TEST(Initialization, DymamicRows2DInitializerList)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DymamicRows2DInitializerList_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ { { 7, 3, 1 }, { 8, 8, 2 } },
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DInitializerList_InitializerColumnsMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRows2DRange_Range2DColumnsMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8 } };
		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, DymamicRows2DInitializerList_SizeMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRows2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Copy_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto rng_2d    = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ rng_2d, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Copy_SizeMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3 }, { 8, 8 } };

		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRows2DRange_Move)
	{
		auto rng_2d       = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Move_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		auto rng_2d          = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ std::move(rng_2d), allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Move_SizeMismatch_Throw)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3 }, { 8, 8 } };

		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ std::move(rng_2d) }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRows_ExprObject_SameColumns)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows_ExprObject_SameColumns_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto dummy     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr      = mpp_test::dummy_expr{ dummy };

		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ expr, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows_ExprObject_DifferentColumns_Throw)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3 }, { 8, 8 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		EXPECT_THROW((mpp::matrix<int, std::dynamic_extent, 3>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRowsDefaultConstructor)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DynamicRowsDefaultConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{};

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, DynamicRowsConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ allocator };

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, DynamicRowsSizeConstructor)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DynamicRowsSizeConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ 2, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DymamicRowsValueConstructor)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 2, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, DynamicRowsValueConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ 2, 0, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DynamicRowsIdentityMatrix)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 3, mpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicRowsIdentityMatrix_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ 3,
            mpp::identity_matrix,
            allocator };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicRowsCallable)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ 2, iota };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, DynamicRowsCallable_CustomAllocator)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, std::dynamic_extent, 3, mpp_test::custom_allocator<int>>{ 2, iota, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, DynamicRowsIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 3>{ 4, mpp::identity_matrix }),
			std::invalid_argument);
	}

	TEST(Initialization, DynamicRowsIdentityMatrix_0x0_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, std::dynamic_extent, 0>{ 0, mpp::identity_matrix }),
			std::invalid_argument);
	}

	/**
	 * Dynamic columns matrices
	 */

	TEST(Initialization, DynamicColumnsCopyConstruct)
	{
		const auto dummy  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = dummy;

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumnsCopyConstruct_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto dummy     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ dummy, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumnsMoveConstruct)
	{
		auto dummy        = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = std::move(dummy);

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumnsMoveConstruct_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		auto dummy           = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ std::move(dummy), allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

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

	TEST(Initialization, DynamicColumns2DInitializerList)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DInitializerList_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ { { 7, 3, 1 }, { 8, 8, 2 } },
				allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DInitializerList_InitializerColumnsMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8 } }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumns2DRange_Range2DColumnsMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8 } };
		EXPECT_THROW((void)(mpp::matrix<int, 2, std::dynamic_extent>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumns2DInitializerList_SizeMismatch_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 1, 2, 3 } }),
			std::invalid_argument);
	}

	TEST(Initialization, DynamicColumns2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Copy_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto rng_2d    = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ rng_2d, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Copy_SizeMismatch_Throw)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 1, 2, 3 } };

		EXPECT_THROW((void)(mpp::matrix<int, 2, std::dynamic_extent>{ rng_2d }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumns2DRange_Move)
	{
		auto rng_2d       = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Move_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		auto rng_2d          = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ std::move(rng_2d), allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Move_SizeMismatch_Throw)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 1, 2, 3 } };

		EXPECT_THROW((void)(mpp::matrix<int, 2, std::dynamic_extent>{ std::move(rng_2d) }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumns_ExprObject_SameRows)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns_ExprObject_SameRows_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto dummy     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr      = mpp_test::dummy_expr{ dummy };

		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ expr, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns_ExprObject_DifferentRows_Throw)
	{
		const auto dummy = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = mpp_test::dummy_expr{ dummy };

		EXPECT_THROW((mpp::matrix<int, 3, std::dynamic_extent>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumnsDefaultConstructor)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ 3 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DynamicColumnsDefaultConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, DynamicColumnsConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);
	}

	TEST(Initialization, DynamicColumnsValueConstructor)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ 3, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, DynamicColumnsValueConstructor_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 3, 1, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix)
	{
		const auto matrix = mpp::matrix<int, 3, std::dynamic_extent>{ 3, mpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix_CustomAllocator)
	{
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix    = mpp::matrix<int, 3, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 3,
            mpp::identity_matrix,
            allocator };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicColumnsCallable)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};

		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ 3, iota };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, DynamicColumnsCallable_CustomAllocator)
	{
		auto iota = [i = 0]() mutable {
			return i++;
		};
		const auto allocator = mpp_test::custom_allocator<int>{};
		const auto matrix =
			mpp::matrix<int, 2, std::dynamic_extent, mpp_test::custom_allocator<int>>{ 3, iota, allocator };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix.get_allocator(), allocator);

		EXPECT_THAT(matrix, ElementsAre(0, 1, 2, 3, 4, 5));
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 3, std::dynamic_extent>{ 4, mpp::identity_matrix }),
			std::invalid_argument);
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix_0x0_Throw)
	{
		EXPECT_THROW((void)(mpp::matrix<int, 0, std::dynamic_extent>{ 0, mpp::identity_matrix }),
			std::invalid_argument);
	}

	// @TODO: Test all initialization overloads with custom allocators
} // namespace