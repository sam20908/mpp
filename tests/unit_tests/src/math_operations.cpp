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

namespace
{
	TEST(Operation, Addition_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrix + matrix;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Addition_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix<int, 3, 3>{ matrix + matrix };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Subtraction_LazyEvaluated)
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

	TEST(Operation, Subtraction_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix<int, 3, 3>{ matrix - matrix };

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

	TEST(Operation, Multiplication_MatrixXScalar_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrix * 2;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Multiplication_MatrixXScalar_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix<int, 3, 3>{ matrix * 2 };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Multiplication_ScalarXMatrix_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = 2 * matrix;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Multiplication_ScalarXMatrix_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix<int, 3, 3>{ 2 * matrix };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 14);
		EXPECT_EQ(result(0, 1), 6);
		EXPECT_EQ(result(0, 2), 2);
		EXPECT_EQ(result(1, 0), 16);
		EXPECT_EQ(result(1, 1), 16);
		EXPECT_EQ(result(1, 2), 4);
		EXPECT_EQ(result(2, 0), 10);
		EXPECT_EQ(result(2, 1), 16);
		EXPECT_EQ(result(2, 2), 4);
	}

	TEST(Operation, Multiplication_MatrixXMatrix_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrix * matrix;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 78);
		EXPECT_EQ(result(0, 1), 53);
		EXPECT_EQ(result(0, 2), 15);
		EXPECT_EQ(result(1, 0), 130);
		EXPECT_EQ(result(1, 1), 104);
		EXPECT_EQ(result(1, 2), 28);
		EXPECT_EQ(result(2, 0), 109);
		EXPECT_EQ(result(2, 1), 95);
		EXPECT_EQ(result(2, 2), 25);
	}

	TEST(Operation, Multiplication_MatrixXMatrix_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix{ matrix * matrix };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 78);
		EXPECT_EQ(result(0, 1), 53);
		EXPECT_EQ(result(0, 2), 15);
		EXPECT_EQ(result(1, 0), 130);
		EXPECT_EQ(result(1, 1), 104);
		EXPECT_EQ(result(1, 2), 28);
		EXPECT_EQ(result(2, 0), 109);
		EXPECT_EQ(result(2, 1), 95);
		EXPECT_EQ(result(2, 2), 25);
	}

	TEST(Operation, Division_MatrixXScalar_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<float, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrix / 2.F;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 3.5F);
		EXPECT_EQ(result(0, 1), 1.5F);
		EXPECT_EQ(result(0, 2), 0.5F);
		EXPECT_EQ(result(1, 0), 4.F);
		EXPECT_EQ(result(1, 1), 4.F);
		EXPECT_EQ(result(1, 2), 1.F);
		EXPECT_EQ(result(2, 0), 2.5F);
		EXPECT_EQ(result(2, 1), 4.F);
		EXPECT_EQ(result(2, 2), 1.F);
	}

	TEST(Operation, Division_MatrixXScalar_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<float, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix{ matrix / 2.F };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 3.5F);
		EXPECT_EQ(result(0, 1), 1.5F);
		EXPECT_EQ(result(0, 2), 0.5F);
		EXPECT_EQ(result(1, 0), 4.F);
		EXPECT_EQ(result(1, 1), 4.F);
		EXPECT_EQ(result(1, 2), 1.F);
		EXPECT_EQ(result(2, 0), 2.5F);
		EXPECT_EQ(result(2, 1), 4.F);
		EXPECT_EQ(result(2, 2), 1.F);
	}

	TEST(Operation, Division_ScalarXMatrix_LazyEvaluated)
	{
		auto matrix = matrixpp::matrix<float, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = 2.F / matrix;

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 3.5F);
		EXPECT_EQ(result(0, 1), 1.5F);
		EXPECT_EQ(result(0, 2), 0.5F);
		EXPECT_EQ(result(1, 0), 4.F);
		EXPECT_EQ(result(1, 1), 4.F);
		EXPECT_EQ(result(1, 2), 1.F);
		EXPECT_EQ(result(2, 0), 2.5F);
		EXPECT_EQ(result(2, 1), 4.F);
		EXPECT_EQ(result(2, 2), 1.F);
	}

	TEST(Operation, Division_ScalarXMatrix_ConstructedMatrix)
	{
		auto matrix = matrixpp::matrix<float, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto result = matrixpp::matrix{ 2.F / matrix };

		EXPECT_EQ(result.rows(), 3);
		EXPECT_EQ(result.columns(), 3);

		EXPECT_EQ(result(0, 0), 3.5F);
		EXPECT_EQ(result(0, 1), 1.5F);
		EXPECT_EQ(result(0, 2), 0.5F);
		EXPECT_EQ(result(1, 0), 4.F);
		EXPECT_EQ(result(1, 1), 4.F);
		EXPECT_EQ(result(1, 2), 1.F);
		EXPECT_EQ(result(2, 0), 2.5F);
		EXPECT_EQ(result(2, 1), 4.F);
		EXPECT_EQ(result(2, 2), 1.F);
	}
} // namespace