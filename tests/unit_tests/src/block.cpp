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

#include <matrixpp/algorithm/block.hpp>
#include <matrixpp/matrix.hpp>

#include <span>
#include <stdexcept>

namespace
{
	using ::testing::ElementsAre;

	/**
	 * Full block crop
	 */

	TEST(Block, FullBlock_FullyStatic)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(2));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Block, FullBlock_FullyDynamic)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(2));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Block, FullBlock_DynamicRows)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(2));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	TEST(Block, FullBlock_DynamicColumns)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(2));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 3);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 1, 8, 8, 2));
	}

	/**
	 * Crop 2x3 to 2x2
	 */

	TEST(Block, Cropped_2x3To2x2_FullyStatic)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(1));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 2);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 8, 8));
	}

	TEST(Block, Cropped_2x3To2x2_FullyDynamic)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(1));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 2);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 8, 8));
	}

	TEST(Block, Cropped_2x3To2x2_DynamicRows)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(1));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 2);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 8, 8));
	}

	TEST(Block, Cropped_2x3To2x2_DynamicColumns)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(1),
            static_cast<std::size_t>(1));

		EXPECT_EQ(to.rows(), 2);
		EXPECT_EQ(to.columns(), 2);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7, 3, 8, 8));
	}

	/**
	 * Crop 2x3 to 1x1
	 */

	TEST(Block, Cropped_2x3To1x1_FullyStatic)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0));

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 1);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7));
	}

	TEST(Block, Cropped_2x3To1x1_FullyDynamic)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0));

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 1);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7));
	}

	TEST(Block, Cropped_2x3To1x1_DynamicRows)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0));

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 1);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7));
	}

	TEST(Block, Cropped_2x3To1x1_DynamicColumns)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		auto to   = matrixpp::block(from,
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0),
            static_cast<std::size_t>(0));

		EXPECT_EQ(to.rows(), 1);
		EXPECT_EQ(to.columns(), 1);

		EXPECT_EQ(to.rows_extent(), std::dynamic_extent);
		EXPECT_EQ(to.columns_extent(), std::dynamic_extent);

		EXPECT_THAT(to, ElementsAre(7));
	}

	/**
	 * Top row index out of range
	 */

	TEST(Block, TopRowIndexOutOfBounds_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowIndexOutOfBounds_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowIndexOutOfBounds_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowIndexOutOfBounds_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	/**
	 * Top column index out of range
	 */

	TEST(Block, TopColumnIndexOutOfBounds_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(3), // Out of range
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnIndexOutOfBounds_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(3), // Out of range
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnIndexOutOfBounds_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(3), // Out of range
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnIndexOutOfBounds_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(3), // Out of range
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	/**
	 * Bottom row index out of range
	 */

	TEST(Block, BottomRowIndexOutOfBounds_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, BottomRowIndexOutOfBounds_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, BottomRowIndexOutOfBounds_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, BottomRowIndexOutOfBounds_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2), // Out of range
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	/**
	 * Bottom column index out of range
	 */

	TEST(Block, BottomColumnIndexOutOfBounds_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(3)), // Out of range
			std::invalid_argument);
	}

	TEST(Block, BottomColumnIndexOutOfBounds_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(3)), // Out of range
			std::invalid_argument);
	}

	TEST(Block, BottomColumnIndexOutOfBounds_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(3)), // Out of range
			std::invalid_argument);
	}

	TEST(Block, BottomColumnIndexOutOfBounds_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1),
						 static_cast<std::size_t>(3)), // Out of range
			std::invalid_argument);
	}

	/**
	 * Top row index bigger than bottom row index
	 */

	TEST(Block, TopRowBiggerThanBottomRow_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(1), // Overlaps bottom row index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowBiggerThanBottomRow_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(1), // Overlaps bottom row index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowBiggerThanBottomRow_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(1), // Overlaps bottom row index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	TEST(Block, TopRowBiggerThanBottomRow_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(1), // Overlaps bottom row index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(2)),
			std::invalid_argument);
	}

	/**
	 * Top column index bigger than bottom column index
	 */

	TEST(Block, TopColumnBiggerThanBottomColumn_FullyStatic_Throw)
	{
		auto from = matrixpp::matrix<int, 2, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1), // Overlaps bottom column index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnBiggerThanBottomColumn_FullyDynamic_Throw)
	{
		auto from = matrixpp::matrix<int>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1), // Overlaps bottom column index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnBiggerThanBottomColumn_DynamicRows_Throw)
	{
		auto from = matrixpp::matrix<int, std::dynamic_extent, 3>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1), // Overlaps bottom column index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0)),
			std::invalid_argument);
	}

	TEST(Block, TopColumnBiggerThanBottomColumn_DynamicColumns_Throw)
	{
		auto from = matrixpp::matrix<int, 2, std::dynamic_extent>{ { 7, 3, 1 }, { 8, 8, 2 } };
		EXPECT_THROW((void)matrixpp::block(from,
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(1), // Overlaps bottom column index "0"
						 static_cast<std::size_t>(0),
						 static_cast<std::size_t>(0)),
			std::invalid_argument);
	}
} // namespace
