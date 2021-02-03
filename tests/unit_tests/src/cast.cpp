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

#include <matrixpp/utility/cast.hpp>
#include <matrixpp/matrix.hpp>

#include <span>

namespace
{
	using ::testing::ElementsAre;

	/**
	 * Cast to different type
	 */

	TEST(Cast, CastToDifferentType_FullyStatic)
	{
		auto from = matrixpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<int>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), 2);
		EXPECT_EQ(to.columns_extent(), 3);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Cast, CastToDifferentType_FullyDynamic)
	{
		auto from = matrixpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<int>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Cast, CastToDifferentType_DynamicRows)
	{
		auto from = matrixpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<int>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), 3);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Cast, CastToDifferentType_DynamicColumns)
	{
		auto from = matrixpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<int>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), 2);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	/**
	 * Cast to same type (more of an edge case test)
	 */

	TEST(Cast, CastToSameType_FullyStatic)
	{
		auto from = matrixpp::matrix<float, 2, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<float>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), 2);
		EXPECT_EQ(to.columns_extent(), 3);

		EXPECT_THAT(to, ElementsAre(7.F, 3.F, 1.F, 8.F, 8.F, 2.F));
	}

	TEST(Cast, CastToSameType_FullyDynamic)
	{
		auto from = matrixpp::matrix<float>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<float>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7.F, 3.F, 1.F, 8.F, 8.F, 2.F));
	}

	TEST(Cast, CastToSameType_DynamicRows)
	{
		auto from = matrixpp::matrix<float, std::dynamic_extent, 3>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<float>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), 3);

		EXPECT_THAT(to, ElementsAre(7.F, 3.F, 1.F, 8.F, 8.F, 2.F));
	}

	TEST(Cast, CastToSameType_DynamicColumns)
	{
		auto from = matrixpp::matrix<float, 2, std::dynamic_extent>{ { 7.F, 3.F, 1.F }, { 8.F, 8.F, 2.F } };
		auto to   = matrixpp::cast(std::type_identity<float>{}, from);

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), 2);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7.F, 3.F, 1.F, 8.F, 8.F, 2.F));
	}
} // namespace