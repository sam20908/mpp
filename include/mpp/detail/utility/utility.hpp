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

#include <compare>
#include <cstddef>
#include <limits>
#include <vector>

namespace mpp
{
	namespace detail
	{
		template<typename Val>
		concept arithmetic = requires(Val&& value)
		{
			{ value + value };
			{ value - value };
			{ value * value };
			{ value / value };
			{ value += value };
			{ value -= value };
			{ value *= value };
			{ value /= value };
		};
	} // namespace detail

	template<detail::arithmetic, std::size_t, std::size_t, typename>
	class mat;

	namespace detail
	{
		template<typename Fn, typename Return, typename... Args>
		concept fn_with_return_type =
			std::invocable<Fn, Args...> && std::same_as<std::invoke_result_t<Fn, Args...>, Return>;

		template<typename Rng>
		using range_2d_value_t = std::ranges::range_value_t<std::ranges::range_value_t<Rng>>;

		template<typename Rng, typename Val>
		concept rng_2d_with_value_convertible_to = std::convertible_to<range_2d_value_t<Rng>, Val>;

		template<typename Rng, typename Val>
		concept rng_1d_with_value_convertible_to = std::convertible_to<std::ranges::range_value_t<Rng>, Val>;

		template<typename>
		struct is_matrix : std::false_type
		{
		};

		template<arithmetic Val, std::size_t Rows, std::size_t Cols, typename Alloc>
		struct is_matrix<mat<Val, Rows, Cols, Alloc>> : std::true_type
		{
		};

		template<typename T, typename To>
		concept matrix_with_value_convertible_to = is_matrix<std::remove_cvref_t<T>>::value &&
			std::convertible_to<typename std::remove_cvref_t<T>::value_type, To>;

		// Double was tested to be *at least* accurate enough to do calculations involve irrational fractions (e.g. 1/3)
		// and
		// provide accurate enough results
		using fp_t = double;

		template<typename T>
		struct is_vec : std::false_type
		{
		};

		template<typename T, typename Alloc>
		struct is_vec<std::vector<T, Alloc>> : std::true_type
		{
		};

		[[nodiscard]] constexpr auto idx_1d(std::size_t cols, std::size_t row, std::size_t col) noexcept -> std::size_t
		{
			// This is mainly for avoiding bug-prone code, because this calculation occurs in a lot of places, and a
			// typo can cause a lot of things to fail. It's safer to wrap this calculation in a function, so the bug is
			// easier to spot. This also assumes that the storage of row-major

			return row * cols + col;
		}

		template<typename Buf>
		void resize_buf_if_vec(Buf& buf,
			std::size_t rows,
			std::size_t cols,
			auto val) // @TODO: ISSUE #20
		{
			if constexpr (is_vec<Buf>::value)
			{
				buf.resize(rows * cols, val);
			}
		}

		template<typename Buf>
		void reserve_buf_if_vec(Buf& buffer, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			if constexpr (is_vec<Buf>::value)
			{
				buffer.reserve(rows * cols);
			}
		}

		template<typename Buf>
		void init_identity_buf(Buf& buffer,
			std::size_t rows,
			std::size_t cols,
			auto zero_val,
			auto one_val) // @TODO: ISSUE #20
		{
			if constexpr (is_vec<Buf>::value)
			{
				buffer.resize(rows * cols, zero_val);
			}

			for (auto index = std::size_t{}; index < rows; ++index)
			{
				buffer[idx_1d(cols, index, index)] = one_val;
			}
		}

		template<typename T>
		constexpr auto constexpr_abs(T t) noexcept -> T
		{
			if (std::is_constant_evaluated())
			{
				return t < 0 ? -t : t;
			}

			return std::abs(t);
		}

		template<typename T>
		constexpr auto fp_is_eq(T a, T b) noexcept -> bool
		{
			return constexpr_abs(a - b) < std::numeric_limits<T>::epsilon();
		}
	} // namespace detail
} // namespace mpp