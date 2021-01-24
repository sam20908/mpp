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

#include <cmath>
#include <cstddef>
#include <limits>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace matrixpp::detail
{
	/**
     * This is mainly for avoiding bug-prone code, because this calculation occurs
     * in a lot of places, and a typo can cause a lot of things to fail. It's
     * safer to wrap this calculation in a function, so the bug is easier to spot
     */
	[[nodiscard]] constexpr std::size_t idx_2d_to_1d(std::size_t cols, std::size_t row_idx, std::size_t col_idx)
	{
		return row_idx * cols + col_idx;
	}

	[[nodiscard]] constexpr auto dimension_not_zero_and_non_zero(std::size_t rows, std::size_t columns)
	{
		return (rows == 0 && columns != 0) || (rows != 0 && columns == 0);
	}

	[[nodiscard]] constexpr auto range_2d_dimensions(auto&& rng_2d)
	{
		auto begin = std::ranges::begin(rng_2d);
		auto rows  = std::ranges::size(rng_2d);
		auto cols  = rows > 0 ? std::ranges::size(*begin) : 0;

		if (rows > 1)
		{
			// We only need to check for equal row columns when the rows is 2+ because
			// 1 row means there's no other row to check and 0 rows is self explanatory

			for (auto&& row : rng_2d)
			{
				if (auto row_cols = std::ranges::size(row); row_cols != cols)
				{
					throw std::invalid_argument("2D initializer doesn't have equal row columns!");
				}
			}
		}

		return std::pair{ rows, cols };
	}

	[[nodiscard]] constexpr auto prefer_static_extent(std::size_t left_extent, std::size_t right_extent)
	{
		if (left_extent != std::dynamic_extent || right_extent != std::dynamic_extent)
		{
			return left_extent != std::dynamic_extent ? left_extent : right_extent;
		}
		else
		{
			return std::dynamic_extent;
		}
	}

	inline void validate_matrices_same_size(const auto& left, const auto& right) // @TODO: ISSUE #20
	{
		if (left.rows() != right.rows() || left.columns() != right.columns())
		{
			throw std::runtime_error("Both matrices don't have the same size!");
		}
	}

	inline void validate_matrices_multipliable(const auto& left, const auto& right) // @TODO: ISSUE #20
	{
		if (left.columns() != right.rows())
		{
			throw std::runtime_error("Left matrix's columns is not equal to right matrix's rows!");
		}
	}

	inline void allocate_1d_buf_if_vector(auto& buf, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
	{
		constexpr auto is_vec = requires
		{
			buf.push_back(0);
		};

		if constexpr (is_vec)
		{
			buf.resize(rows * cols);
		}
	}

	template<typename Value>
	inline void transform_1d_buf_into_identity(auto& buf, std::size_t n) // @TODO: ISSUE #20
	{
		for (auto idx = std::size_t{ 0 }; idx < n; ++idx)
		{
			buf[idx_2d_to_1d(n, idx, idx)] = Value{ 1 };
		}
	}

	template<typename To, typename From>
	inline void mul_square_bufs(auto& buf, auto&& l_buf, auto&& r_buf, std::size_t n)
	{
		for (auto row = std::size_t{ 0 }; row < n; ++row)
		{
			for (auto col = std::size_t{ 0 }; col < n; ++col)
			{
				auto result = From{ 0 };

				for (auto elem = std::size_t{ 0 }; elem < n; ++elem)
				{
					const auto left_idx  = idx_2d_to_1d(n, row, elem);
					const auto right_idx = idx_2d_to_1d(n, elem, col);

					result += l_buf[left_idx] * r_buf[right_idx];
				}

				auto idx = idx_2d_to_1d(n, row, col);
				buf[idx] = static_cast<To>(result);
			}
		}
	}

	template<typename T>
	constexpr auto accurate_equals(T left, T right) -> bool
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			const auto epsilon = std::numeric_limits<T>::epsilon();
			return std::abs(left - right) < epsilon;
		}
		else
		{
			return left == right;
		}
	}
} // namespace matrixpp::detail