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


#include <mpp/detail/utility/utility.hpp>

#include <cstddef>

namespace mpp::detail
{
	[[nodiscard]] constexpr auto prefer_static_extent(std::size_t left_extent, std::size_t right_extent) -> std::size_t
	{
		if (left_extent != dynamic || right_extent != dynamic)
		{
			return left_extent != dynamic ? left_extent : right_extent;
		}
		else
		{
			return dynamic;
		}
	}

	inline void forward_substitution_in_place(auto& l_buffer, std::size_t columns) // @TODO: ISSUE #20
	{
		for (auto col = std::size_t{ 1 }; col < columns; ++col)
		{
			// Optimized version of forward-substitution which skips making diagnoal 1's because L would've already
			// had 1's along the diagonal

			for (auto row = col + 1; row < columns; ++row)
			{
				const auto factor = l_buffer[index_2d_to_1d(columns, row, row - 1)] * -1;

				for (auto col_2 = std::size_t{}; col_2 < col; ++col_2)
				{
					const auto elem_above = l_buffer[index_2d_to_1d(columns, col, col_2)];
					const auto elem_index = index_2d_to_1d(columns, row, col_2);

					l_buffer[elem_index] -= factor * elem_above;
				}
			}
		}
	}

	inline void back_substitution_in_place(auto& u_buffer, std::size_t columns) // @TODO: ISSUE #20
	{
		for (auto col = columns; col > 0; --col)
		{
			const auto col_index = col - 1;
			auto diag_elem_index = index_2d_to_1d(columns, col_index, col_index);
			const auto diag_elem = u_buffer[diag_elem_index];

			// Diagonal element can simply be replaced with the factor
			const auto diag_factor    = default_floating_type{ 1 } / diag_elem;
			u_buffer[diag_elem_index] = diag_factor;

			// Multiply every element to the right of the diagonal element by the factor
			for (auto index = columns - col; index > 0; --index)
			{
				u_buffer[++diag_elem_index] *= diag_factor;
			}

			for (auto row = col_index; row > 0; --row)
			{
				// Use the diagonal element as the factor to compute the numbers above the pivot in the same column
				// (this works because) the augmented matrix would have zeroes above the diagonal element
				const auto row_index          = row - 1;
				const auto elem_index         = index_2d_to_1d(columns, row_index, col_index);
				const auto elem_before_factor = u_buffer[elem_index];
				u_buffer[elem_index]          = elem_before_factor * diag_factor * -1;

				// Add the corresponding elements of the rows of the current diagonal element onto the rows above
				for (auto col_2 = columns; col_2 > col; --col_2)
				{
					const auto col_2_index   = col_2 - 1;
					auto diag_row_index      = index_2d_to_1d(columns, col_index, col_2_index);
					const auto diag_row_elem = u_buffer[diag_row_index];

					const auto cur_elem_index = index_2d_to_1d(columns, row_index, col_2_index);
					const auto elem           = u_buffer[cur_elem_index];
					const auto factor         = elem_before_factor * -1;

					const auto new_elem      = factor * diag_row_elem + elem;
					u_buffer[cur_elem_index] = new_elem;
				}
			}
		}
	}

	template<typename To, typename From>
	inline void matrix_multiplication_on_buffers(auto& out_buffer,
		const auto& left_buffer,
		const auto& right_buffer,
		std::size_t left_rows,
		std::size_t left_columns,
		std::size_t right_rows,
		std::size_t right_columns) // @TODO: ISSUE #20
	{
		for (auto left_row = std::size_t{}; left_row < left_rows; ++left_row)
		{
			for (auto right_column = std::size_t{}; right_column < right_columns; ++right_column)
			{
				auto result = To{};

				for (auto index = std::size_t{}; index < left_columns; ++index)
				{
					const auto left_index  = index_2d_to_1d(right_rows, left_row, index);
					const auto right_index = index_2d_to_1d(right_rows, index, right_column);

					result += static_cast<To>(left_buffer[left_index]) * static_cast<To>(right_buffer[right_index]);
				}

				const auto result_element_index  = index_2d_to_1d(left_columns, left_row, right_column);
				out_buffer[result_element_index] = result;
			}
		}
	}

	template<typename To, bool FillLBuf>
	inline auto lu_decomposition_and_compute_determinant_in_place(std::size_t rows,
		std::size_t columns,
		auto& l_buffer,
		auto& u_buffer) -> To // @TODO: ISSUE #20
	{
		// Things this function expects from l_buffer and u_buffer:
		// 1. l_buffer is already an identity buffer
		// 2. u_buffer has the original values

		auto det = To{ 1 };

		// Compute the determinant while also compute LU Decomposition
		for (auto row = std::size_t{}; row < rows; ++row)
		{
			auto begin_index     = index_2d_to_1d(columns, row, std::size_t{});
			const auto end_index = index_2d_to_1d(columns, row, columns);

			for (auto col = std::size_t{}; col < row; ++col)
			{
				// This allows us to keep track of the row of the factor later on without having to manually
				// calculate from indexes
				auto factor_row_index = index_2d_to_1d(columns, col, col);

				const auto elem_index = index_2d_to_1d(columns, row, col);
				const auto factor     = u_buffer[elem_index] / u_buffer[factor_row_index] * -1;

				for (auto index = begin_index; index < end_index; ++index)
				{
					u_buffer[index] += factor * u_buffer[factor_row_index++];
				}

				// Handle cases where we don't need to store the factors in a separate buffer (plain determinant
				// algorithm for example). This allows passing empty buffer as l_buffer for optimization
				if constexpr (FillLBuf)
				{
					// L stores the opposite (opposite sign) of the factors used for U in the corresponding
					// location. But, to help optimize the calculation of inv(L), we can just leave the sign because
					// all the diagnoal elements below the diagonal element by 1 are the opposite sign, and it's
					// relatively easy to fix the values of other factors
					l_buffer[elem_index] = factor;
				}

				++begin_index;
			}

			const auto diag_elem_index = index_2d_to_1d(columns, row, row);
			det *= u_buffer[diag_elem_index];
		}

		return det;
	}
} // namespace mpp::detail