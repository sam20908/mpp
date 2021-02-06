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

#include <matrixpp/matrix.hpp>

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

	TEST(Initialization, FullyStatic2DInitializerList)
	{
		const auto matrix = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DRange_Move)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, 3>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DArray_Copy)
	{
		const auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, 3>{ arr_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic2DArray_Move)
	{
		const auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, 3>{ std::move(arr_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic_StaticExprObject)
	{
		const auto dummy = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		const auto expr   = matrixpp_test::detail::dummy_expr{ dummy };
		const auto matrix = matrixpp::matrix<int, 2, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic_DynamicExprObject_SameSize)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		const auto matrix = matrixpp::matrix<int, 2, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyStatic_DynamicExprObject_DifferentSize_Throw)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		EXPECT_THROW((matrixpp::matrix<int, 3, 3>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, FullyStaticDefaultConstructor)
	{
		const auto matrix = matrixpp::matrix<int, 2, 3>{};

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, FullyStaticValueConstructor)
	{
		const auto matrix = matrixpp::matrix<int, 2, 3>{ 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, FullyStaticIdentityMatrix)
	{
		const auto matrix = matrixpp::matrix<int, 3, 3>{ matrixpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, FullyStaticIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(matrixpp::matrix<int, 3, 4>{ matrixpp::identity_matrix }), std::invalid_argument);
	}

	/**
	 * Fully dynamic matrices
	 */

	TEST(Initialization, FullyDynamic2DInitializerList)
	{
		const auto matrix = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic2DRange_Move)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamic_ExprObject)
	{
		// Since fully dynamic matrices take any sized expression object, it is not possible for them to throw an
		// exception, therefore no need to write a separate test for that

		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		const auto matrix = matrixpp::matrix<int>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, FullyDynamicDefaultConstructor)
	{
		const auto matrix = matrixpp::matrix<int>{};

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);
	}

	TEST(Initialization, FullyDynamicValueConstructor)
	{
		const auto matrix = matrixpp::matrix<int>{ 2, 3, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, FullyDynamicIdentityMatrix)
	{
		const auto matrix = matrixpp::matrix<int>{ 3, 3, matrixpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, FullyDynamicIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(matrixpp::matrix<int>{ 3, 4, matrixpp::identity_matrix }), std::invalid_argument);
	}

	/**
	 * Dynamic rows matrices
	 */

	TEST(Initialization, DymamicRowsSDInitializerList)
	{
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows2DRange_Move)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}


	TEST(Initialization, DynamicRows_ExprObject_SameColumns)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicRows_ExprObject_DifferentColumns_Throw)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3 }, { 8, 8 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		EXPECT_THROW((matrixpp::matrix<int, std::dynamic_extent, 3>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, DynamicRowsDefaultConstructor)
	{
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ 2 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DymamicRowsValueConstructor)
	{
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ 2, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, DynamicRowsIdentityMatrix)
	{
		const auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ 3, matrixpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicRowsIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(matrixpp::matrix<int, std::dynamic_extent, 3>{ 4, matrixpp::identity_matrix }),
			std::invalid_argument);
	}

	/**
	 * Dynamic columns matrices
	 */

	TEST(Initialization, DynamicColumns2DInitializerList)
	{
		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Copy)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns2DRange_Move)
	{
		const auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ std::move(rng_2d) };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns_ExprObject_SameRows)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ expr };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Initialization, DynamicColumns_ExprObject_DifferentRows_Throw)
	{
		const auto dummy = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr  = matrixpp_test::detail::dummy_expr{ dummy };

		EXPECT_THROW((matrixpp::matrix<int, 3, std::dynamic_extent>{ expr }), std::invalid_argument);
	}

	TEST(Initialization, DynamicColumnsDefaultConstructor)
	{
		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ 3 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Initialization, DynamicColumnsValueConstructor)
	{
		const auto matrix = matrixpp::matrix<int, 2, std::dynamic_extent>{ 3, 1 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 2);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 1, 1, 1, 1, 1));
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix)
	{
		const auto matrix = matrixpp::matrix<int, 3, std::dynamic_extent>{ 3, matrixpp::identity_matrix };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(matrix, ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}

	TEST(Initialization, DynamicColumnsIdentityMatrix_NotSquare_Throw)
	{
		EXPECT_THROW((void)(matrixpp::matrix<int, 3, std::dynamic_extent>{ 4, matrixpp::identity_matrix }),
			std::invalid_argument);
	}
} // namespace