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

#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/utility/comparison.hpp>

#include <cmath>
#include <compare>
#include <cstddef>
#include <type_traits>

namespace mpp::detail
{
	template<typename Mat, typename T>
	using mat_rebind_to_t = matrix<T,
		Mat::rows_extent(),
		Mat::columns_extent(),
		typename std::allocator_traits<typename Mat::allocator_type>::template rebind_alloc<T>>;

	[[nodiscard]] constexpr auto prefer_static_extent(std::size_t left_extent, std::size_t right_extent) noexcept
		-> std::size_t
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

	template<typename Value>
	[[nodiscard]] constexpr auto fp_is_zero_or_nan(const Value& val) -> bool
	{
		using ordering = std::compare_three_way_result_t<Value, Value>;

		return floating_point_compare(val, Value{}) == ordering::equivalent || std::isnan(val);
	}

	template<typename To, bool FillLBuffer, bool CalculateDeterminant>
	inline auto lu_generic(std::size_t rows, std::size_t columns, auto& l_buffer, auto& u_buffer)
		-> To // @TODO: ISSUE #20
	{
		// Shortcut method from
		// https://medium.com/linear-algebra-basics/lu-decomposition-c8f9b75ddeff

		// Things this function expects from l_buffer and u_buffer:
		// 1. l_buffer is already an identity buffer
		// 2. u_buffer has the original values

		auto det = default_floating_type{ 1 };

		for (auto row = std::size_t{}; row < rows; ++row)
		{
			// Micro-optimization: allow other indexes to reference this instead of multi-step calculation
			const auto diag_front_index = index_2d_to_1d(columns, row, std::size_t{});
			const auto diag_index       = diag_front_index + row;
			const auto diag_elem        = u_buffer[diag_index];

			for (auto inner_row = row + 1; inner_row < rows; ++inner_row)
			{
				const auto inner_front_index = index_2d_to_1d(columns, inner_row, row);
				const auto inner_front_elem  = u_buffer[inner_front_index];

				for (auto column = row; column < columns; ++column)
				{
					const auto inner_row_current_index = index_2d_to_1d(columns, inner_row, column);

					// row as column parameter means accessing diagnoal element
					const auto inner_row_current_elem      = u_buffer[inner_row_current_index];
					const auto diag_row_corresponding_elem = u_buffer[diag_front_index + column];

					// inner_row_current_elem - (inner_front_elem * diag_row_corresponding_elem) / diag_elem

					const auto result_elem =
						inner_row_current_elem - (inner_front_elem * diag_row_corresponding_elem) / diag_elem;

					u_buffer[inner_row_current_index] = result_elem;
				};

				if constexpr (FillLBuffer)
				{
					// We don't necessarily need to fill the l_buffer if we're only getting the determinant because only
					// u_buffer will be used
					l_buffer[inner_front_index] = inner_front_elem / diag_elem;
				};
			}

			if constexpr (CalculateDeterminant)
			{
				det *= u_buffer[diag_index];
			}
		}

		return static_cast<To>(det);
	}
} // namespace mpp::detail
