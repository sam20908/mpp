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

#include <matrixpp/utility/equal.hpp>
#include <matrixpp/matrix.hpp>

namespace
{
	TEST(Equality, SameSize_SameData_SameExtent)
	{
		auto left  = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto right = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_EQ(left, right);
	}

	TEST(Equality, SameSize_DifferentData_SameExtent)
	{
		auto left  = matrixpp::matrix<int, 3, 3>{ { 7, 4, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto right = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 1, 2 }, { 5, 8, 1 } };

		EXPECT_NE(left, right);
	}

	TEST(Equality, DifferentSize_FullyStaticMatrices)
	{
		auto left  = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto right = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_NE(left, right);
	}

	TEST(Equality, DifferentSize_FullyDynamicMatrices)
	{
		auto left  = matrixpp::matrix{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto right = matrixpp::matrix{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_NE(left, right);
	}

	TEST(Equality, DifferentSize_DifferentKindsMatrices)
	{
		auto left  = matrixpp::matrix{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto right = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };

		EXPECT_NE(left, right);
	}
} // namespace