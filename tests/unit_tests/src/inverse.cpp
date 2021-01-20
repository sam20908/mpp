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
#include <matrixpp/algorithm/inverse.hpp>
#include <matrixpp/matrix.hpp>
#include <utility>

namespace
{
	TEST(Inverse, 3x3)
	{
		auto matrix  = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
		auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_FLOAT_EQ(inverse(0, 0), 0.F);
		EXPECT_FLOAT_EQ(inverse(0, 1), 1.F / 3.F);
		EXPECT_FLOAT_EQ(inverse(0, 2), -1.F / 3.F);
		EXPECT_FLOAT_EQ(inverse(1, 0), -1.F);
		EXPECT_FLOAT_EQ(inverse(1, 1), 3.F / 2.F);
		EXPECT_FLOAT_EQ(inverse(1, 2), -1.F);
		EXPECT_FLOAT_EQ(inverse(2, 0), 4.F);
		EXPECT_FLOAT_EQ(inverse(2, 1), -41.F / 6.F);
		EXPECT_FLOAT_EQ(inverse(2, 2), 16.F / 3.F);
	}

	TEST(Inverse, 0x0_FullyStatic_Edgecase)
	{
		auto matrix  = matrixpp::matrix<int, 0, 0>{};
		auto inverse = matrixpp::inverse(std::type_identity<float>{}, matrix);

		EXPECT_EQ(inverse.rows_extent(), 1);
		EXPECT_EQ(inverse.columns_extent(), 1);

		EXPECT_FLOAT_EQ(inverse(0, 0), 1.F);
	}
} // namespace