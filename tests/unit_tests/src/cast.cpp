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
#include <matrixpp/utility/cast.hpp>
#include <span>

namespace
{
	TEST(TypeCasting, FullyStatic)
	{
		auto from = matrixpp::matrix<int, 3, 3>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::cast<float>(from);

		EXPECT_EQ(to(0, 0), 7.F);
		EXPECT_EQ(to(0, 1), 3.F);
		EXPECT_EQ(to(0, 2), 1.F);
		EXPECT_EQ(to(1, 0), 8.F);
		EXPECT_EQ(to(1, 1), 8.F);
		EXPECT_EQ(to(1, 2), 2.F);
		EXPECT_EQ(to(2, 0), 5.F);
		EXPECT_EQ(to(2, 1), 8.F);
		EXPECT_EQ(to(2, 2), 2.F);
	}

	TEST(TypeCasting, FullyDynamic)
	{
		auto from = matrixpp::matrix<int>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::cast<float>(from);

		EXPECT_EQ(to(0, 0), 7.F);
		EXPECT_EQ(to(0, 1), 3.F);
		EXPECT_EQ(to(0, 2), 1.F);
		EXPECT_EQ(to(1, 0), 8.F);
		EXPECT_EQ(to(1, 1), 8.F);
		EXPECT_EQ(to(1, 2), 2.F);
		EXPECT_EQ(to(2, 0), 5.F);
		EXPECT_EQ(to(2, 1), 8.F);
		EXPECT_EQ(to(2, 2), 2.F);
	}

	TEST(TypeCasting, DynamicColumns)
	{
		auto from = matrixpp::matrix<int, 3, std::dynamic_extent>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::cast<float>(from);

		EXPECT_EQ(to(0, 0), 7.F);
		EXPECT_EQ(to(0, 1), 3.F);
		EXPECT_EQ(to(0, 2), 1.F);
		EXPECT_EQ(to(1, 0), 8.F);
		EXPECT_EQ(to(1, 1), 8.F);
		EXPECT_EQ(to(1, 2), 2.F);
		EXPECT_EQ(to(2, 0), 5.F);
		EXPECT_EQ(to(2, 1), 8.F);
		EXPECT_EQ(to(2, 2), 2.F);
	}

	TEST(TypeCasting, DynamicRows)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::cast<float>(from);

		EXPECT_EQ(to(0, 0), 7.F);
		EXPECT_EQ(to(0, 1), 3.F);
		EXPECT_EQ(to(0, 2), 1.F);
		EXPECT_EQ(to(1, 0), 8.F);
		EXPECT_EQ(to(1, 1), 8.F);
		EXPECT_EQ(to(1, 2), 2.F);
		EXPECT_EQ(to(2, 0), 5.F);
		EXPECT_EQ(to(2, 1), 8.F);
		EXPECT_EQ(to(2, 2), 2.F);
	}
} // namespace