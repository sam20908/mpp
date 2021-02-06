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

#include <mpp/arithmetic.hpp>
#include <mpp/matrix.hpp>

#include <span>
#include <stdexcept>

namespace
{
	using ::testing::ElementsAre;

	/**
	 * Addition
	 */

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyStaticAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyStaticAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyStaticAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyStaticAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyStaticAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_FullyDynamicAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyDynamicAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyDynamicAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyDynamicAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_FullyDynamicAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicRowsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicRowsAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicRowsAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicRowsAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicRowsAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix + matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_SameSizedOperands_DynamicColumnsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix + matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicColumnsAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicColumnsAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicColumnsAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Addition_DifferentSizedOperands_DynamicColumnsAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix + matrix2), std::invalid_argument);
	}

	/**
	 * Subtraction
	 */

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyStaticAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyStaticAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyStaticAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyStaticAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyStaticAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_FullyDynamicAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyDynamicAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyDynamicAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyDynamicAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_FullyDynamicAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicRowsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicRowsAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicRowsAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicRowsAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicRowsAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr    = matrix - matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 0);
		EXPECT_EQ(expr(0, 1), 0);
		EXPECT_EQ(expr(0, 2), 0);
		EXPECT_EQ(expr(1, 0), 0);
		EXPECT_EQ(expr(1, 1), 0);
		EXPECT_EQ(expr(1, 2), 0);
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_SameSizedOperands_DynamicColumnsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix - matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(0, 0, 0, 0, 0, 0));
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicColumnsAndFullyStatic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicColumnsAndFullyDynamic_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicColumnsAndDynamicRows_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Subtraction_DifferentSizedOperands_DynamicColumnsAndDynamicColumns_Throw)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 6, 7, 8 } };
		EXPECT_THROW((void)(matrix - matrix2), std::invalid_argument);
	}

	/**
	 * Multiplication
	 */

	TEST(Arithmetic, Multiplication_MatrixXScalar_FullyStatic_Expr)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = matrix * scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_FullyDynamic_Expr)
	{
		const auto matrix = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = matrix * scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_DynamicRows_Expr)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = matrix * scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_DynamicColumns_Expr)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = matrix * scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_FullyStatic_Expr)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = scalar * matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_FullyDynamic_Expr)
	{
		const auto matrix = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = scalar * matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_DynamicRows_Expr)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = scalar * matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_DynamicColumns_Expr)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar = 2;
		const auto expr   = scalar * matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_FullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ matrix * scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_FullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ matrix * scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_DynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ matrix * scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_MatrixXScalar_DynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ matrix * scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_FullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ scalar * matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_FullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ scalar * matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_DynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ scalar * matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_ScalarXMatrix_DynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto scalar      = 2;
		const auto expr_matrix = mpp::matrix{ scalar * matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(14, 6, 2, 16, 16, 4));
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyStatic_Expr)
	{
		auto matrix        = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyDynamic_Expr)
	{
		auto matrix        = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicRows_Expr)
	{
		auto matrix        = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicColumns_Expr)
	{
		auto matrix        = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyStatic_Expr)
	{
		auto matrix        = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyDynamic_Expr)
	{
		auto matrix        = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicRows_Expr)
	{
		auto matrix        = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicColumns_Expr)
	{
		auto matrix        = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyStatic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyDynamic_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicRows_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 2);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicColumns_Expr)
	{
		const auto matrix  = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr    = matrix * matrix2;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 2);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 60);
		EXPECT_EQ(expr(0, 1), 47);
		EXPECT_EQ(expr(1, 0), 80);
		EXPECT_EQ(expr(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, std::dynamic_extent, 2>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 2);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2     = mpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3 }, { 1, 8 }, { 8, 2 } };
		const auto expr_matrix = mpp::matrix{ matrix * matrix2 };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 2);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr_matrix(0, 0), 60);
		EXPECT_EQ(expr_matrix(0, 1), 47);
		EXPECT_EQ(expr_matrix(1, 0), 80);
		EXPECT_EQ(expr_matrix(1, 1), 92);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyStatic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndFullyDynamic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicRows_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyStaticAndDynamicColumns_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyStatic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndFullyDynamic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicRows_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_FullyDynamicAndDynamicColumns_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyStatic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndFullyDynamic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicRows_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicRowsAndDynamicColumns_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyStatic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndFullyDynamic_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicRows_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	TEST(Arithmetic, Multiplication_MatrixXMatrix_DynamicColumnsAndDynamicColumns_NotMultipliableMatrices_Throw)
	{
		const auto matrix1 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		const auto matrix2 = mpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::invalid_argument);
	}

	/**
	 * Division
	 */

	TEST(Arithmetic, Division_MatrixXScalar_FullyStatic_Expr)
	{
		const auto matrix = mpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = matrix / scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_MatrixXScalar_FullyDynamic_Expr)
	{
		const auto matrix = mpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = matrix / scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_MatrixXScalar_DynamicRows_Expr)
	{
		const auto matrix = mpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = matrix / scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_MatrixXScalar_DynamicColumns_Expr)
	{
		const auto matrix = mpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = matrix / scalar;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_ScalarXMatrix_FullyStatic_Expr)
	{
		const auto matrix = mpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = scalar / matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_ScalarXMatrix_FullyDynamic_Expr)
	{
		const auto matrix = mpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = scalar / matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_ScalarXMatrix_DynamicRows_Expr)
	{
		const auto matrix = mpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = scalar / matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr.columns_extent(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_ScalarXMatrix_DynamicColumns_Expr)
	{
		const auto matrix = mpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar = 2.F;
		const auto expr   = scalar / matrix;

		EXPECT_EQ(expr.rows(), 2);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr.rows_extent(), 2);
		EXPECT_EQ(expr.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
	}

	TEST(Arithmetic, Division_MatrixXScalar_FullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ matrix / scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_MatrixXScalar_FullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ matrix / scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_MatrixXScalar_DynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ matrix / scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_MatrixXScalar_DynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ matrix / scalar };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_ScalarXMatrix_FullyStatic_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ scalar / matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_ScalarXMatrix_FullyDynamic_Evaluated)
	{
		const auto matrix      = mpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ scalar / matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_ScalarXMatrix_DynamicRows_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ scalar / matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(expr_matrix.columns_extent(), 3);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}

	TEST(Arithmetic, Division_ScalarXMatrix_DynamicColumns_Evaluated)
	{
		const auto matrix      = mpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		const auto scalar      = 2.F;
		const auto expr_matrix = mpp::matrix{ scalar / matrix };

		EXPECT_EQ(expr_matrix.rows(), 2);
		EXPECT_EQ(expr_matrix.columns(), 3);

		EXPECT_EQ(expr_matrix.rows_extent(), 2);
		EXPECT_EQ(expr_matrix.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(expr_matrix, ElementsAre(3.5F, 1.5F, 0.5F, 4.F, 4.F, 1.F));
	}
} // namespace