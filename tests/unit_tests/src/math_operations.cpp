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
#include <matrixpp/matrix.hpp>
#include <matrixpp/operation.hpp>
#include <span>
#include <stdexcept>

namespace
{
	TEST(Operation, Addition_SameSizedOperands)
	{
		auto matrix = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = matrix + matrix;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
		EXPECT_EQ(expr(2, 0), 10);
		EXPECT_EQ(expr(2, 1), 16);
		EXPECT_EQ(expr(2, 2), 4);
	}

	TEST(Operation, Addition_DifferentSizedOperands_Throw)
	{
		auto matrix1 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 }, { 6, 7, 8 } };
		auto matrix2 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		EXPECT_THROW((void)(matrix1 + matrix2), std::runtime_error);
	}

	TEST(Operation, Subtraction_SameSizedOperands)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrix - matrix;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 0);
		EXPECT_EQ(result(0, 1), 0);
		EXPECT_EQ(result(0, 2), 0);
		EXPECT_EQ(result(1, 0), 0);
		EXPECT_EQ(result(1, 1), 0);
		EXPECT_EQ(result(1, 2), 0);
		EXPECT_EQ(result(2, 0), 0);
		EXPECT_EQ(result(2, 1), 0);
		EXPECT_EQ(result(2, 2), 0);
	}

	TEST(Operation, Subtraction_DifferentSizedOperands_Throw)
	{
		auto matrix1 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 }, { 6, 7, 8 } };
		auto matrix2 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		EXPECT_THROW((void)(matrix1 - matrix2), std::runtime_error);
	}

	TEST(Operation, Multiplication_MatrixXScalar)
	{
		auto matrix = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = matrix * 2;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
		EXPECT_EQ(expr(2, 0), 10);
		EXPECT_EQ(expr(2, 1), 16);
		EXPECT_EQ(expr(2, 2), 4);
	}

	TEST(Operation, Multiplication_ScalarXMatrix)
	{
		auto matrix = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = 2 * matrix;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 14);
		EXPECT_EQ(expr(0, 1), 6);
		EXPECT_EQ(expr(0, 2), 2);
		EXPECT_EQ(expr(1, 0), 16);
		EXPECT_EQ(expr(1, 1), 16);
		EXPECT_EQ(expr(1, 2), 4);
		EXPECT_EQ(expr(2, 0), 10);
		EXPECT_EQ(expr(2, 1), 16);
		EXPECT_EQ(expr(2, 2), 4);
	}

	TEST(Operation, Multiplication_MatrixXMatrix_MultipliableMatrices)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = matrix * matrix;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 78);
		EXPECT_EQ(expr(0, 1), 53);
		EXPECT_EQ(expr(0, 2), 15);
		EXPECT_EQ(expr(1, 0), 130);
		EXPECT_EQ(expr(1, 1), 104);
		EXPECT_EQ(expr(1, 2), 28);
		EXPECT_EQ(expr(2, 0), 109);
		EXPECT_EQ(expr(2, 1), 95);
		EXPECT_EQ(expr(2, 2), 25);
	}

	TEST(Operation, Multiplication_MatrixXMatrix_NotMultipliableMatrices_Throw)
	{
		auto matrix1 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix2 = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 }, { 1, 1, 1 } };
		EXPECT_THROW((void)(matrix1 * matrix2), std::runtime_error);
	}

	TEST(Operation, Division_MatrixXScalar)
	{
		auto matrix = matrixpp::matrix<float>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = matrix / 2.F;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
		EXPECT_EQ(expr(2, 0), 2.5F);
		EXPECT_EQ(expr(2, 1), 4.F);
		EXPECT_EQ(expr(2, 2), 1.F);
	}

	TEST(Operation, Division_ScalarXMatrix)
	{
		auto matrix = matrixpp::matrix<float>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto expr   = 2.F / matrix;

		EXPECT_EQ(expr.rows(), 3);
		EXPECT_EQ(expr.columns(), 3);

		EXPECT_EQ(expr(0, 0), 3.5F);
		EXPECT_EQ(expr(0, 1), 1.5F);
		EXPECT_EQ(expr(0, 2), 0.5F);
		EXPECT_EQ(expr(1, 0), 4.F);
		EXPECT_EQ(expr(1, 1), 4.F);
		EXPECT_EQ(expr(1, 2), 1.F);
		EXPECT_EQ(expr(2, 0), 2.5F);
		EXPECT_EQ(expr(2, 1), 4.F);
		EXPECT_EQ(expr(2, 2), 1.F);
	}

	TEST(Operation, Addition_PreferStaticExtent)
	{
		auto matrix1 = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix2 = matrixpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result  = matrixpp::matrix{ matrix1 + matrix2 };

		EXPECT_EQ(result.rows_extent(), 3);
		EXPECT_EQ(result.columns_extent(), 3);
	}

	TEST(Operation, Subtraction_PreferStaticExtent)
	{
		auto matrix1 = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix2 = matrixpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result  = matrixpp::matrix{ matrix1 - matrix2 };

		EXPECT_EQ(result.rows_extent(), 3);
		EXPECT_EQ(result.columns_extent(), 3);
	}
} // namespace