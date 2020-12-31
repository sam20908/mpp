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

#pragma once

#include "../detail/utility.hpp"
#include "../matrix.hpp"
#include "../utility/square.hpp"

#include <algorithm>
#include <ranges>

namespace matrixpp
{
	namespace detail
	{
		template<typename Precision>
		[[nodiscard]] Precision determinant_impl(auto& data,
			std::size_t rows,
			std::size_t columns,
			std::size_t row_begin,
			std::size_t column_end)
		{
			// The size of the data stays the same, so we have to determine the actual
			// size of the data we're allowed to work with by using column_end
			auto columns_range = column_end + 1;

			if (columns_range == 0)
			{
				return static_cast<Precision>(1);
			}
			else if (columns_range == 1)
			{
				// We don't have to worry about row_begin because this case won't happen
				// unless the *starting* matrix is 1x1

				return static_cast<Precision>(data[0]);
			}
			else if (columns_range == 2)
			{
				// We start at row_begin because that's the range of rows we're working
				// with. This matters when we're working with matrices bigger than 2x2
				// because the 2x2 matrix in there doesn't start at the top, and row_begin
				// helps defining the index which the inner matrix is intended to start

				// Columns aren't affected by this, because the way we rearrange the row
				// elements, the intended elements always start at the beginning

				auto top_left_index     = idx_2d_to_1d(columns, row_begin, 0);
				auto top_right_index    = idx_2d_to_1d(columns, row_begin, 1);
				auto bottom_left_index  = idx_2d_to_1d(columns, row_begin + 1, 0);
				auto bottom_right_index = idx_2d_to_1d(columns, row_begin + 1, 1);

				auto ad =
					static_cast<Precision>(data[top_left_index]) * static_cast<Precision>(data[bottom_right_index]);
				auto bc =
					static_cast<Precision>(data[top_right_index]) * static_cast<Precision>(data[bottom_left_index]);

				return ad - bc;
			}
			else
			{
				auto result = static_cast<Precision>(0);
				auto sign   = static_cast<Precision>(1);

				// 3x3 and higher. Use recursion

				/*
				* Instead of manually recreating the entire inner matrix for calculation,
				* we can just rotate and swap the elements of the untrimmed inner matrix
				* (the matrix which we haven't "cut off" a column). With this method, we
				* can just ignore the last column of this untrimmed matrix. Here is an
				* example of this in action:
				*
				* Suppose we start with this 4x4 matrix:
				* [1 2 3 4]
				* [5 6 7 8]
				* [9 8 7 6]
				* [5 4 3 2]
				*
				* We can already cut off the top row in trivial fashion by using row_begin,
				* so we can start off by cutting the first column. We're just going to work
				* with this matrix where the top row got cut off: [5 6 7 8] [9 8 7 6] [5 4
				* 3 2]
				*
				* Cutting off the first column would *always* start by rotating the row
				* left. This can be achieved by using `std::rotate`: [6 7 8 5] [8 7 6 9] [4
				* 3 2 5]
				*
				* Then ignoring the second column would be swapping the first element and
				* the last element. [5 7 8 6] [9 7 6 8] [5 3 2 4]
				*
				* Ignoring the third column would be swapping the second element and the
				* last element. [5 6 8 7] [9 8 6 7] [5 4 2 3]
				*
				* Igoring the fourth column would be swapping the third element and the
				* last element. This would also go back to where we started! [5 6 7 8] [9 8
				* 7 6] [5 4 3 2]
				*
				* This is important because we guarantee that all minor determinants are
				* based on their "base" matrix, and the base matrix's values are back to
				* where it should be. Suprisingly, this also guarantees that all minor
				* determinant calculations are going to restore the base matrix back to
				* where it should be
				*/

				for (auto column_index = std::size_t{ 0 }; column_index <= column_end; ++column_index)
				{
					if (column_index == 0)
					{
						for (auto row_index = row_begin + 1; row_index < rows; ++row_index)
						{
							auto row_begin_index = idx_2d_to_1d(columns, row_index, 0);

							auto row_begin          = std::next(data.begin(), row_begin_index);
							auto row                = std::views::counted(row_begin, column_end + 1);
							auto row_second_element = std::next(row_begin, 1);

							std::ranges::rotate(row, row_second_element);
						}
					}
					else
					{
						for (auto row_index = row_begin + 1; row_index < rows; ++row_index)
						{
							auto row_begin_index = idx_2d_to_1d(columns, row_index, 0);

							auto row_begin        = std::next(data.begin(), row_begin_index);
							auto left_element_it  = std::next(row_begin, column_index - 1);
							auto right_element_it = std::next(row_begin, column_end);

							std::iter_swap(left_element_it, right_element_it);
						}
					}

					// We effectively "shrink" the working range in the recursion call by
					// making the minor start at the row below the current row_begin and
					// "removing" the last column
					auto minor_determinant =
						determinant_impl<Precision>(data, rows, columns, row_begin + 1, column_end - 1);

					auto coefficient_index = idx_2d_to_1d(columns, row_begin, column_index);

					result += static_cast<Precision>(data[coefficient_index]) * minor_determinant * sign;
					sign *= -1;
				}

				return result;
			}
		}
	} // namespace detail

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline auto determinant(const matrix<Value, RowsExtent, ColumnsExtent>& obj) // @TODO: ISSUE #20
	{
		if (!square(obj))
		{
			throw std::runtime_error("Cannot find determinant of a non-square matrix!");
		}

		auto buf_copy = obj.buffer();
		auto rows     = obj.rows();
		auto cols     = obj.columns();

		return detail::determinant_impl<Value>(buf_copy, rows, cols, 0, cols - 1);
	}

	template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline auto determinant(const matrix<Value, RowsExtent, ColumnsExtent>& obj) // @TODO: ISSUE #20
	{
		if (!square(obj))
		{
			throw std::runtime_error("Cannot find determinant of a non-square matrix!");
		}

		auto buf_copy = obj.buffer();
		auto rows     = obj.rows();
		auto cols     = obj.columns();

		return detail::determinant_impl<To>(buf_copy, rows, cols, 0, cols - 1);
	}
} // namespace matrixpp