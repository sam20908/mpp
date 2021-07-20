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
#include <mpp/mat/matfwd.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace mpp::detail
{
	inline constexpr auto dummy = 0;

	template<typename Mat, typename T>
	using mat_rebind_to_t = mat<T,
		Mat::rows_extent(),
		Mat::cols_extent(),
		typename std::allocator_traits<typename Mat::allocator_type>::template rebind_alloc<T>>;

	template<typename Val>
	[[nodiscard]] constexpr auto fp_is_zero_or_nan(Val val) -> bool
	{
		return fp_is_eq(val, Val{}) || std::isnan(val);
	}

	template<typename To, bool FillL, bool GetDet>
	inline auto lu_impl(std::size_t rows, std::size_t cols, auto& l, auto& u) -> To // @TODO: ISSUE #20
	{
		// Shortcut method from
		// https://medium.com/linear-algebra-basics/lu-decomposition-c8f9b75ddeff

		// Things this function expects from l and u:
		// 1. l is already an identity buffer
		// 2. u has the original values

		auto det_ = fp_t{ 1 };

		for (auto row = std::size_t{}; row < rows; ++row)
		{
			// Micro-optimization: allow other indexes to reference this instead of multi-step calculation
			const auto diag_front_idx = idx_1d(cols, row, std::size_t{});
			const auto diag_idx       = diag_front_idx + row;
			const auto diag           = u[diag_idx];

			for (auto inner_row = row + 1; inner_row < rows; ++inner_row)
			{
				const auto inner_front_idx = idx_1d(cols, inner_row, row);
				const auto inner_front     = u[inner_front_idx];

				for (auto col = row; col < cols; ++col)
				{
					const auto inner_row_current_index = idx_1d(cols, inner_row, col);

					// row as column parameter means accessing diagnoal element
					const auto inner_row_cur = u[inner_row_current_index];
					const auto diag_row_cor  = u[diag_front_idx + col];

					const auto res = inner_row_cur - (inner_front * diag_row_cor) / diag;

					u[inner_row_current_index] = res;
				};

				if constexpr (FillL)
				{
					// We don't necessarily need to fill the l if we're only getting the determinant because only
					// u will be used
					l[inner_front_idx] = inner_front / diag;
				};
			}

			if constexpr (GetDet)
			{
				det_ *= u[diag_idx];
			}
		}

		return static_cast<To>(det_);
	}

	template<typename Buf>
	inline auto back_sub_buf(const auto& a, const auto& b, std::size_t n) -> Buf // @TODO: ISSUE #20
	{
		auto x_buf = Buf{};

		// @TODO: Any way to make this utilize push_back?
		resize_buf_if_vec(x_buf, n, 1, fp_t{});

		/**
		 * Implementation of back substitution from
		 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-back-backward-substitution/
		 */
		for (auto row = n; row > std::size_t{}; --row)
		{
			const auto row_idx = row - 1;

			auto res = static_cast<fp_t>(b[idx_1d(1, row_idx, 0)]);

			for (auto col = n - 1; col > row_idx; --col)
			{
				res -= a[idx_1d(n, row_idx, col)] * x_buf[col];
			}

			const auto diag = static_cast<fp_t>(a[idx_1d(n, row_idx, row_idx)]);

			assert(!fp_is_zero_or_nan(diag));

			res /= diag;

			x_buf[row_idx] = res;
		}

		return x_buf;
	}

	template<typename Buf>
	inline auto forward_sub_buf(const auto& a, const auto& b, std::size_t n) -> Buf // @TODO: ISSUE #20
	{
		auto x_buf = Buf{};

		resize_buf_if_vec(x_buf, n, 1, fp_t{});

		/**
		 * Implementation of forward substitution from
		 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-forward-backward-substitution/
		 */
		for (auto row = std::size_t{}; row < n; ++row)
		{
			auto result = static_cast<fp_t>(b[idx_1d(1, row, 0)]);

			for (auto col = std::size_t{}; col < row; ++col)
			{
				result -= a[idx_1d(n, row, col)] * x_buf[col];
			}

			const auto diag = static_cast<fp_t>(a[idx_1d(n, row, row)]);

			assert(!fp_is_zero_or_nan(diag));

			result /= diag;

			x_buf[row] = result;
		}

		return x_buf;
	}
} // namespace mpp::detail
