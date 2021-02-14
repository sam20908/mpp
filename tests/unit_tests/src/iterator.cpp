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

#include <mpp/algorithm/inverse.hpp>
#include <mpp/matrix.hpp>

namespace
{
	/**
	 * Not mutating operations
	 */

	TEST(Iterator, StaticIterator_MutableMatrix_Dereference)
	{
		auto matrix = mpp::matrix<int, 1, 1>{ { 1 } };

		const auto begin  = matrix.begin();
		const auto cbegin = matrix.begin();

		EXPECT_EQ(*begin, 1);
		EXPECT_EQ(*cbegin, 1);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_Dereference)
	{
		const auto matrix = mpp::matrix<int, 1, 1>{ { 1 } };

		const auto begin  = matrix.begin();
		const auto cbegin = matrix.begin();

		EXPECT_EQ(*begin, 1);
		EXPECT_EQ(*cbegin, 1);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_IndexAccess)
	{
		auto matrix       = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();

		EXPECT_EQ(begin[1], 2);
		EXPECT_EQ(cbegin[1], 2);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_IndexAccess)
	{
		const auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();

		EXPECT_EQ(begin[1], 2);
		EXPECT_EQ(cbegin[1], 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_Comparisons)
	{
		auto matrix       = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_LE(begin, end);
		EXPECT_LE(cbegin, cend);
		EXPECT_NE(begin, end);
		EXPECT_NE(cbegin, cend);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_Comparisons_Spaceship)
	{
		auto matrix       = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(begin <=> end, std::strong_ordering::less);
		EXPECT_EQ(cbegin <=> cend, std::strong_ordering::less);
		EXPECT_EQ(begin <=> begin, std::strong_ordering::equal);
		EXPECT_EQ(cbegin <=> cbegin, std::strong_ordering::equal);
		EXPECT_EQ(end <=> end, std::strong_ordering::equal);
		EXPECT_EQ(cend <=> cend, std::strong_ordering::equal);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_Comparisons)
	{
		const auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_LE(begin, end);
		EXPECT_LE(cbegin, cend);
		EXPECT_NE(begin, end);
		EXPECT_NE(cbegin, cend);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_Comparisons_Spaceship)
	{
		const auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(begin <=> end, std::strong_ordering::less);
		EXPECT_EQ(cbegin <=> cend, std::strong_ordering::less);
		EXPECT_EQ(begin <=> begin, std::strong_ordering::equal);
		EXPECT_EQ(cbegin <=> cbegin, std::strong_ordering::equal);
		EXPECT_EQ(end <=> end, std::strong_ordering::equal);
		EXPECT_EQ(cend <=> cend, std::strong_ordering::equal);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_Difference)
	{
		auto matrix       = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(end - begin, 2);
		EXPECT_EQ(cend - cbegin, 2);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_Difference)
	{
		const auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(end - begin, 2);
		EXPECT_EQ(cend - cbegin, 2);
		EXPECT_EQ(begin - cbegin, 0);
		EXPECT_EQ(end - cend, 0);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_Dereference)
	{
		auto matrix = mpp::matrix<int>{ { 1 } };

		const auto begin  = matrix.begin();
		const auto cbegin = matrix.begin();

		EXPECT_EQ(*begin, 1);
		EXPECT_EQ(*cbegin, 1);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_Dereference)
	{
		const auto matrix = mpp::matrix<int>{ { 1 } };

		const auto begin  = matrix.begin();
		const auto cbegin = matrix.begin();

		EXPECT_EQ(*begin, 1);
		EXPECT_EQ(*cbegin, 1);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_IndexAccess)
	{
		auto matrix       = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();

		EXPECT_EQ(begin[1], 2);
		EXPECT_EQ(cbegin[1], 2);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_IndexAccess)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();

		EXPECT_EQ(begin[1], 2);
		EXPECT_EQ(cbegin[1], 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_Comparisons)
	{
		auto matrix       = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_LE(begin, end);
		EXPECT_LE(cbegin, cend);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_Comparisons_Spaceship)
	{
		auto matrix       = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(begin <=> end, std::strong_ordering::less);
		EXPECT_EQ(cbegin <=> cend, std::strong_ordering::less);
		EXPECT_EQ(begin <=> begin, std::strong_ordering::equal);
		EXPECT_EQ(cbegin <=> cbegin, std::strong_ordering::equal);
		EXPECT_EQ(end <=> end, std::strong_ordering::equal);
		EXPECT_EQ(cend <=> cend, std::strong_ordering::equal);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_Comparisons)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_LE(begin, end);
		EXPECT_LE(cbegin, cend);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_Comparisons_Spaceship)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(begin <=> end, std::strong_ordering::less);
		EXPECT_EQ(cbegin <=> cend, std::strong_ordering::less);
		EXPECT_EQ(begin <=> begin, std::strong_ordering::equal);
		EXPECT_EQ(cbegin <=> cbegin, std::strong_ordering::equal);
		EXPECT_EQ(end <=> end, std::strong_ordering::equal);
		EXPECT_EQ(cend <=> cend, std::strong_ordering::equal);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_Difference)
	{
		auto matrix       = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(end - begin, 2);
		EXPECT_EQ(cend - cbegin, 2);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_Difference)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 } };
		const auto begin  = matrix.begin();
		const auto cbegin = matrix.cbegin();
		const auto end    = matrix.end();
		const auto cend   = matrix.cend();

		EXPECT_EQ(end - begin, 2);
		EXPECT_EQ(cend - cbegin, 2);
		EXPECT_EQ(begin - cbegin, 0);
		EXPECT_EQ(end - cend, 0);
	}

	/**
	 * Mutating operations
	 */

	TEST(Iterator, StaticIterator_MutableMatrix_PrefixIncrement)
	{
		auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };

		auto begin  = matrix.begin();
		auto cbegin = matrix.begin();

		++begin;
		++cbegin;

		EXPECT_EQ(*begin, 2);
		EXPECT_EQ(*cbegin, 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_PostfixIncrement)
	{
		auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };

		auto begin      = matrix.begin();
		auto cbegin     = matrix.begin();
		auto begin_old  = begin++;
		auto cbegin_old = cbegin++;

		EXPECT_EQ(*begin_old, 1);
		EXPECT_EQ(*cbegin_old, 1);
		EXPECT_EQ(*begin, 2);
		EXPECT_EQ(*cbegin, 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_IncrementByPlusEquals)
	{
		auto matrix = mpp::matrix<int, 1, 3>{ { 1, 2, 3 } };

		auto begin  = matrix.begin();
		auto cbegin = matrix.begin();

		begin += 2;
		cbegin += 2;

		EXPECT_EQ(*begin, 3);
		EXPECT_EQ(*cbegin, 3);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_PrefixDecrement)
	{
		auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };

		auto end  = matrix.end();
		auto cend = matrix.cend();

		--end;
		--cend;

		EXPECT_EQ(*end, 2);
		EXPECT_EQ(*cend, 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_PostfixDecrement)
	{
		auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };

		auto end      = matrix.end() - 1;
		auto cend     = matrix.cend() - 1;
		auto end_old  = end--;
		auto cend_old = cend--;

		EXPECT_EQ(*end_old, 2);
		EXPECT_EQ(*cend_old, 2);
		EXPECT_EQ(*end, 1);
		EXPECT_EQ(*cend, 1);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_DecrementByMinusEquals)
	{
		auto matrix = mpp::matrix<int, 1, 3>{ { 1, 2, 3 } };

		auto end  = matrix.end();
		auto cend = matrix.cend();

		end -= 3;
		cend -= 3;

		EXPECT_EQ(*end, 1);
		EXPECT_EQ(*cend, 1);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_DereferenceAndWrite)
	{
		auto matrix      = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		const auto begin = matrix.begin();

		*begin = 2;

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_IndexAccessAndWrite)
	{
		auto matrix = mpp::matrix<int, 1, 2>{ { 1, 2 } };
		auto begin  = matrix.begin();

		begin[0] = 2;

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_MoveForwardRows)
	{
		auto matrix = mpp::matrix<int, 2, 2>{ { 1, 2 }, { 3, 4 } };
		auto begin  = matrix.begin();

		begin.move_forward_rows(1);

		EXPECT_EQ(*begin, 3);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_MoveForwardRows)
	{
		const auto matrix = mpp::matrix<int, 2, 2>{ { 1, 2 }, { 3, 4 } };
		auto begin        = matrix.begin();

		begin.move_forward_rows(1);

		EXPECT_EQ(*begin, 3);
	}

	TEST(Iterator, StaticIterator_MutableMatrix_MoveBackwardRows)
	{
		auto matrix = mpp::matrix<int, 2, 2>{ { 1, 2 }, { 3, 4 } };
		auto begin  = matrix.end() - 1;

		begin.move_backward_rows(1);

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, StaticIterator_ImmutableMatrix_MoveBackwardRows)
	{
		const auto matrix = mpp::matrix<int, 2, 2>{ { 1, 2 }, { 3, 4 } };
		auto begin        = matrix.end() - 1;

		begin.move_backward_rows(1);

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_PrefixIncrement)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 } };

		auto begin  = matrix.begin();
		auto cbegin = matrix.begin();

		++begin;
		++cbegin;

		EXPECT_EQ(*begin, 2);
		EXPECT_EQ(*cbegin, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_PostfixIncrement)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 } };

		auto begin      = matrix.begin();
		auto cbegin     = matrix.begin();
		auto begin_old  = begin++;
		auto cbegin_old = cbegin++;

		EXPECT_EQ(*begin_old, 1);
		EXPECT_EQ(*cbegin_old, 1);
		EXPECT_EQ(*begin, 2);
		EXPECT_EQ(*cbegin, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_IncrementByPlusEquals)
	{
		auto matrix = mpp::matrix<int, 1, 3>{ { 1, 2, 3 } };

		auto begin  = matrix.begin();
		auto cbegin = matrix.begin();

		begin += 2;
		cbegin += 2;

		EXPECT_EQ(*begin, 3);
		EXPECT_EQ(*cbegin, 3);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_PrefixDecrement)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 } };

		auto end  = matrix.end();
		auto cend = matrix.cend();

		--end;
		--cend;

		EXPECT_EQ(*end, 2);
		EXPECT_EQ(*cend, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_PostfixDecrement)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 } };

		auto end      = matrix.end() - 1;
		auto cend     = matrix.cend() - 1;
		auto end_old  = end--;
		auto cend_old = cend--;

		EXPECT_EQ(*end_old, 2);
		EXPECT_EQ(*cend_old, 2);
		EXPECT_EQ(*end, 1);
		EXPECT_EQ(*cend, 1);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_DecrementByMinusEquals)
	{
		auto matrix = mpp::matrix<int, 1, 3>{ { 1, 2, 3 } };

		auto end  = matrix.end();
		auto cend = matrix.cend();

		end -= 3;
		cend -= 3;

		EXPECT_EQ(*end, 1);
		EXPECT_EQ(*cend, 1);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_DereferenceAndWrite)
	{
		auto matrix      = mpp::matrix<int>{ { 1, 2 } };
		const auto begin = matrix.begin();

		*begin = 2;

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_IndexAccessAndWrite)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 } };
		auto begin  = matrix.begin();

		begin[0] = 2;

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_MoveForwardRows)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
		auto begin  = matrix.begin();

		begin.move_forward_rows(1);

		EXPECT_EQ(*begin, 3);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_MoveForwardRows)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
		auto begin        = matrix.begin();

		begin.move_forward_rows(1);

		EXPECT_EQ(*begin, 3);
	}

	TEST(Iterator, DynamicIterator_MutableMatrix_MoveBackwardRows)
	{
		auto matrix = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
		auto begin  = matrix.end() - 1;

		begin.move_backward_rows(1);

		EXPECT_EQ(*begin, 2);
	}

	TEST(Iterator, DynamicIterator_ImmutableMatrix_MoveBackwardRows)
	{
		const auto matrix = mpp::matrix<int>{ { 1, 2 }, { 3, 4 } };
		auto begin        = matrix.end() - 1;

		begin.move_backward_rows(1);

		EXPECT_EQ(*begin, 2);
	}
} // namespace