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

#include <array>
#include <gtest/gtest.h>
#include <matrixpp/matrix.hpp>
#include <matrixpp/operation/multiply.hpp>
#include <span>
#include <vector>

namespace
{
	TEST(Initialization, FullyStatic2DInitializerList)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyStatic2DRange)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix = matrixpp::matrix<int, 3, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyStatic2DArray)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto matrix = matrixpp::matrix<int, 3, 3>{ arr_2d };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyStatic_StaticExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix<int, 3, 3>{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, 3, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyStatic_DynamicExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix<int>{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, 3, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyStaticValueConstructor)
	{
		auto matrix = matrixpp::matrix<int, 3, 3>{ 1 };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 1);
		EXPECT_EQ(matrix(0, 1), 1);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 1);
		EXPECT_EQ(matrix(1, 1), 1);
		EXPECT_EQ(matrix(1, 2), 1);
		EXPECT_EQ(matrix(2, 0), 1);
		EXPECT_EQ(matrix(2, 1), 1);
		EXPECT_EQ(matrix(2, 2), 1);
	}

	TEST(Initialization, FullyDynamic2DInitializerList)
	{
		auto matrix = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyDynamic2DRange)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix = matrixpp::matrix<int>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyDynamic_StaticExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix<int, 3, 3>{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyDynamic_DynamicExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, FullyDynamicValueConstructor)
	{
		auto matrix = matrixpp::matrix<int>{ 3, 3, 1 };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 1);
		EXPECT_EQ(matrix(0, 1), 1);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 1);
		EXPECT_EQ(matrix(1, 1), 1);
		EXPECT_EQ(matrix(1, 2), 1);
		EXPECT_EQ(matrix(2, 0), 1);
		EXPECT_EQ(matrix(2, 1), 1);
		EXPECT_EQ(matrix(2, 2), 1);
	}

	TEST(Initialization, DynamicColumns2DInitializerList)
	{
		auto matrix = matrixpp::matrix<int, 3, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicColumns2DRange)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix = matrixpp::matrix<int, 3, std::dynamic_extent>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicColumns_StaticExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix<int, 3, 3>{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicColumns_DynamicExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), 3);
		EXPECT_EQ(matrix.columns_extent(), std::dynamic_extent);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicColumnsValueConstructor)
	{
		auto matrix = matrixpp::matrix<int, 3, std::dynamic_extent>{ 3, 1 };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 1);
		EXPECT_EQ(matrix(0, 1), 1);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 1);
		EXPECT_EQ(matrix(1, 1), 1);
		EXPECT_EQ(matrix(1, 2), 1);
		EXPECT_EQ(matrix(2, 0), 1);
		EXPECT_EQ(matrix(2, 1), 1);
		EXPECT_EQ(matrix(2, 2), 1);
	}

	TEST(Initialization, DymamicRowsSDInitializerList)
	{
		auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicRows2DRange)
	{
		auto rng_2d = std::vector<std::vector<int>>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ rng_2d };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicRows_StaticExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix<int, 3, 3>{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DynamicRows_DynamicExprObject)
	{
		auto arr_2d = std::array{ std::array{ 7, 3, 1 }, std::array{ 8, 8, 2 }, std::array{ 5, 8, 2 } };
		auto m      = matrixpp::matrix{ arr_2d };

		auto expr   = m * 1;
		auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ expr };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(matrix.columns_extent(), 3);

		EXPECT_EQ(matrix(0, 0), 7);
		EXPECT_EQ(matrix(0, 1), 3);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 8);
		EXPECT_EQ(matrix(1, 1), 8);
		EXPECT_EQ(matrix(1, 2), 2);
		EXPECT_EQ(matrix(2, 0), 5);
		EXPECT_EQ(matrix(2, 1), 8);
		EXPECT_EQ(matrix(2, 2), 2);
	}

	TEST(Initialization, DymamicRowsValueConstructor)
	{
		auto matrix = matrixpp::matrix<int, std::dynamic_extent, 3>{ 3, 1 };

		EXPECT_EQ(matrix.rows(), 3);
		EXPECT_EQ(matrix.columns(), 3);

		EXPECT_EQ(matrix(0, 0), 1);
		EXPECT_EQ(matrix(0, 1), 1);
		EXPECT_EQ(matrix(0, 2), 1);
		EXPECT_EQ(matrix(1, 0), 1);
		EXPECT_EQ(matrix(1, 1), 1);
		EXPECT_EQ(matrix(1, 2), 1);
		EXPECT_EQ(matrix(2, 0), 1);
		EXPECT_EQ(matrix(2, 1), 1);
		EXPECT_EQ(matrix(2, 2), 1);
	}
} // namespace