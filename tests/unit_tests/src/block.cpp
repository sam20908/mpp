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
#include <matrixpp/algorithm/block.hpp>
#include <matrixpp/matrix/fully_dynamic.hpp>
#include <stdexcept>

namespace
{
	TEST(Block, FullBlock)
	{
		auto from = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::block(from,
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(2),
			static_cast<std::size_t>(2));

		EXPECT_EQ(to.rows(), 3);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to(0, 0), 7);
		EXPECT_EQ(to(0, 1), 3);
		EXPECT_EQ(to(0, 2), 1);
		EXPECT_EQ(to(1, 0), 8);
		EXPECT_EQ(to(1, 1), 8);
		EXPECT_EQ(to(1, 2), 2);
		EXPECT_EQ(to(2, 0), 5);
		EXPECT_EQ(to(2, 1), 8);
		EXPECT_EQ(to(2, 2), 2);
	}

	TEST(Block, Cropped_3x3To2x2)
	{
		auto from = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::block(from,
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(1),
			static_cast<std::size_t>(1));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 2);

		EXPECT_EQ(to(0, 0), 7);
		EXPECT_EQ(to(0, 1), 3);
		EXPECT_EQ(to(0, 2), 8);
		EXPECT_EQ(to(1, 0), 8);
	}

	TEST(Block, Cropped_3x3To1x1)
	{
		auto from = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		auto to   = matrixpp::block(from,
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0));

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 1);

		EXPECT_EQ(to(0, 0), 8);
	}

	TEST(Block, IndicesOverlap_Throw)
	{
		auto from = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		EXPECT_THROW((void)matrixpp::block(from,
			static_cast<std::size_t>(1),
			static_cast<std::size_t>(1),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0)), std::invalid_argument);
	}

	TEST(Block, IndicesOutOfBounds_Throw)
	{
		auto from = matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };
		EXPECT_THROW((void)matrixpp::block(from,
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(0),
			static_cast<std::size_t>(2),
			static_cast<std::size_t>(3)), std::invalid_argument);
	}
} // namespace
