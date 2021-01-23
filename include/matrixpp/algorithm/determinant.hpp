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

#include "../detail/algo_types.hpp"
#include "../detail/tag_invoke.hpp"
#include "../detail/utility.hpp"
#include "../matrix.hpp"
#include "../utility/square.hpp"

#include <algorithm>
#include <ranges>
#include <utility>

namespace matrixpp
{
	namespace detail
	{
		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto det_lu_decomp(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> To // @TODO: ISSUE #20
		{
			const auto rows = obj.rows();
			const auto cols = obj.columns();

			// Handle special cases - avoid computing LU Decomposition
			if (rows == 0)
			{
				return To{ 1 };
			}
			else if (rows == 1)
			{
				return static_cast<To>(obj(0, 0));
			}
			else if (rows == 2)
			{
				const auto ad = static_cast<To>(obj(0, 0)) * static_cast<To>(obj(1, 1));
				const auto bc = static_cast<To>(obj(0, 1)) * static_cast<To>(obj(1, 0));

				return ad - bc;
			}

			// Store U as floating time to allow accurate determinant
			// long double is used to avoid overflow and underflow as much as possible
			using lu_decomp_matrix_t = matrix<long double, RowsExtent, ColumnsExtent>;
			using lu_decomp_buf_t    = typename lu_decomp_matrix_t::buffer_type;
			using lu_decomp_diff_t   = typename lu_decomp_matrix_t::difference_type;

			auto u_buf = lu_decomp_buf_t{};

			allocate_1d_buf_if_vector(u_buf, rows, cols);
			std::ranges::copy(u_buf, u_buf.begin());

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U)
			// Since det(L) is always 1, we can avoid creating L entirely

			// Compute U
			for (auto row = std::size_t{ 0 }; row < rows; ++row)
			{
				const auto begin_idx = static_cast<lu_decomp_diff_t>(idx_2d_to_1d(cols, row, std::size_t{ 0 }));
				const auto end_idx   = static_cast<lu_decomp_diff_t>(idx_2d_to_1d(cols, row, cols));

				auto begin = std::next(u_buf.begin(), begin_idx);
				auto end   = std::next(u_buf.begin(), end_idx);

				for (auto col = std::size_t{ 0 }; col < row; ++col)
				{
					// This allows us to keep track of the row of the factor
					// later on without having to manually calculate from indexes
					auto factor_idx = idx_2d_to_1d(cols, col, col);

					const auto elem_idx = idx_2d_to_1d(cols, row, col);
					const auto factor   = u_buf[elem_idx] / u_buf[factor_idx] * -1;

					std::ranges::transform(begin, end, begin, [factor, factor_idx, &u_buf](auto elem) mutable {
						return factor * u_buf[factor_idx++] + elem;
					});

					++begin;
				}
			}

			auto det = lu_decomp_value_t{ 1 };
			// The determinant is the product of all pivots of U (diagnoal elements)
			for (auto idx = std::size_t{ 0 }; idx < cols; ++idx)
			{
				const auto elem_idx = idx_2d_to_1d(cols, idx, idx);
				det *= u_buf[elem_idx];
			}

			// Only cast the determinant back to user specified To because using it
			// in the actual computation would've made the error way bigger
			return static_cast<To>(det);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto det_func(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> To // @TODO: ISSUE #20
		{
			if (!square(obj))
			{
				throw std::runtime_error("Cannot find determinant of a non-square matrix!");
			}

			return det_lu_decomp<To>(obj);
		}
	} // namespace detail

	struct determinant_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend constexpr auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj) -> Value
		{
			return detail::det_func<Value>(obj);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend constexpr auto
		tag_invoke(determinant_t, std::type_identity<To>, const matrix<Value, RowsExtent, ColumnsExtent>& obj) -> To
		{
			return detail::det_func<To>(obj);
		}

		template<typename... Args>
		[[nodiscard]] constexpr auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<determinant_t, Args...>
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto determinant = determinant_t{};
} // namespace matrixpp
