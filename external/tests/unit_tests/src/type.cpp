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

#include <mpp/utility/type.hpp>
#include <mpp/matrix.hpp>

#include <span>

namespace
{
	TEST(Type, FullyStatic)
	{
		const auto matrix = mpp::matrix<int, 2, 3>{};

		EXPECT_EQ(mpp::type(matrix), mpp::matrix_type::fully_static);
	}

	TEST(Type, FullyDynamic)
	{
		const auto matrix = mpp::matrix<int>{};

		EXPECT_EQ(mpp::type(matrix), mpp::matrix_type::fully_dynamic);
	}


	TEST(Type, DynamicColumns)
	{
		const auto matrix = mpp::matrix<int, 2, std::dynamic_extent>{};

		EXPECT_EQ(mpp::type(matrix), mpp::matrix_type::dynamic_columns);
	}

	TEST(Type, DynamicRows)
	{
		const auto matrix = mpp::matrix<int, std::dynamic_extent, 3>{};

		EXPECT_EQ(mpp::type(matrix), mpp::matrix_type::dynamic_rows);
	}
} // namespace