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
#include "../detail/matrix_base.hpp"
#include "../detail/tag_invoke.hpp"
#include "../detail/utility.hpp"
#include "../utility/square.hpp"

#include <algorithm>
#include <ranges>
#include <type_traits>

namespace matrixpp
{
	namespace detail
	{
		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto inv_lu_decomp(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent>
		{
			const auto rows = obj.rows();
			const auto cols = obj.columns();
			auto det        = lu_decomp_value_t{ 0 };

			// Handle special cases - compute determinant without LU Decomposition
			if (rows == 0)
			{
				det = lu_decomp_value_t{ 1 };
			}
			else if (rows == 1)
			{
				det = static_cast<lu_decomp_value_t>(obj(0, 0));
			}
			else if (rows == 2)
			{
				const auto ad = static_cast<lu_decomp_value_t>(obj(0, 0)) * static_cast<lu_decomp_value_t>(obj(1, 1));
				const auto bc = static_cast<lu_decomp_value_t>(obj(0, 1)) * static_cast<lu_decomp_value_t>(obj(1, 0));

				det = ad - bc;
			}

			// Early singular check for 2x2 and below matrices to avoid computing
			// LU Decomposition if possible
			if (rows < 3 && accurate_equals(det, lu_decomp_value_t{ 0 }))
			{
				throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
			}

			using inv_matrix_t       = matrix<To, RowsExtent, ColumnsExtent>;
			using lu_decomp_matrix_t = matrix<lu_decomp_value_t, RowsExtent, ColumnsExtent>;
			using lu_decomp_buf_t    = typename lu_decomp_matrix_t::buffer_type;
			using lu_decomp_diff_t   = typename lu_decomp_matrix_t::difference_type;

			auto inv_matrix = inv_matrix_t{};
			auto l_buf      = lu_decomp_buf_t{};
			auto u_buf      = lu_decomp_buf_t{};

			allocate_1d_buf_if_vector(u_buf, rows, cols);
			std::ranges::copy(obj, u_buf.begin());

			allocate_1d_buf_if_vector(l_buf, rows, cols);
			transform_1d_buf_into_identity<lu_decomp_value_t>(l_buf, rows);

			// While computing LU Decomposition, we can compute the determinant,
			// inv(L), and U at the same time
			// The determinant is needed to evaluate the singularity of 3x3
			// and bigger matrices
			det = lu_decomp_value_t{ 1 };
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

					// L stores the opposite (opposite sign) of the factors used for
					// U in the corresponding location, but since inv(A) equals
					// the opposite sign of the non-pivot (diagnal elements) elements,
					// we can directly compute the inverse by not changing the sign of
					// the factor
					l_buf[elem_idx] = factor;

					++begin;
				}

				const auto pivot_idx = idx_2d_to_1d(cols, row, row);
				det *= u_buf[pivot_idx];
			}

			if (rows >= 3 && accurate_equals(det, lu_decomp_value_t{ 0 }))
			{
				throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
			}

			// Handle special cases - avoid LU inverse multiplication
			// and computing inv(U)
			if (rows == 0)
			{
				// Avoiding creating extra buffer by returning early
				return inv_matrix;
			}

			auto inv_matrix_buf = typename inv_matrix_t::buffer_type{};
			allocate_1d_buf_if_vector(inv_matrix_buf, rows, cols);

			if (rows == 1)
			{
				inv_matrix_buf[0] = To{ 1 } / static_cast<To>(obj(0, 0));
			}
			else if (rows == 2)
			{
				const auto element_1 = static_cast<To>(obj(1, 1));
				const auto element_2 = static_cast<To>(obj(0, 1)) * -1;
				const auto element_3 = static_cast<To>(obj(1, 0)) * -1;
				const auto element_4 = static_cast<To>(obj(0, 0));

				inv_matrix_buf[0] = element_1;
				inv_matrix_buf[1] = element_2;
				inv_matrix_buf[2] = element_3;
				inv_matrix_buf[3] = element_4;

				const auto multiplier = To{ 1 } / static_cast<To>(det);
				std::ranges::transform(inv_matrix_buf,
					inv_matrix_buf.begin(),
					std::bind_front(std::multiplies<>{}, multiplier));
			}
			else
			{
				// Compute inverse of U directly on the same buffer
				for (auto col = cols; col > 0; --col)
				{
					// This is used to nagivate across the row where the diagnoal element
					// is
					auto elem_idx = idx_2d_to_1d(cols, col - 1, col - 1);

					const auto elem   = u_buf[elem_idx];
					const auto factor = lu_decomp_value_t{ 1 } / elem;

					u_buf[elem_idx] = factor;
					for (auto idx = cols; idx > col; --idx)
					{
						u_buf[++elem_idx] *= factor;
					}

					for (auto row = col - 1; row > 0; --row)
					{
						const auto cur_elem_idx        = idx_2d_to_1d(cols, row - 1, col - 1);
						const auto factor_row_elem_idx = idx_2d_to_1d(cols, col - 1, col - 1);
						const auto cur_elem            = u_buf[cur_elem_idx];
						const auto cur_factor          = cur_elem * -1;

						u_buf[cur_elem_idx] = cur_factor * u_buf[factor_row_elem_idx];
					}

					for (auto col_2 = cols; col_2 > col; --col_2)
					{
						for (auto row = col - 1; row > 0; --row)
						{
							const auto cur_elem_idx = idx_2d_to_1d(cols, row - 1, col_2 - 1);
							const auto row_idx      = idx_2d_to_1d(cols, col - 1, col_2 - 1);
							const auto factor_idx   = idx_2d_to_1d(cols, row - 1, col - 1);

							const auto factor     = u_buf[factor_idx];
							const auto cur_elem   = u_buf[cur_elem_idx];
							const auto cur_factor = factor * -1;

							u_buf[cur_elem_idx] = cur_factor * u_buf[row_idx] + cur_elem;
						}
					}
				}

				mul_square_bufs<To, lu_decomp_value_t>(inv_matrix_buf, std::move(u_buf), std::move(l_buf), rows);
			}

			init_matrix_base_with_1d_rng(inv_matrix, std::move(inv_matrix_buf), rows, cols);
			return inv_matrix;
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto inv_func(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			if (!square(obj))
			{
				throw std::runtime_error("Inverse of a non-square matrix doesn't exist!");
			}

			return inv_lu_decomp<To>(obj);
		}
	} // namespace detail

	struct inverse_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend constexpr auto tag_invoke(inverse_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<Value, RowsExtent, ColumnsExtent>
		{
			return detail::inv_func<Value>(obj);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend constexpr auto tag_invoke(inverse_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj) -> matrix<To, RowsExtent, ColumnsExtent>
		{
			return detail::inv_func<To>(obj);
		}

		template<typename... Args>
		[[nodiscard]] constexpr auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<inverse_t, Args...>
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto inverse = inverse_t{};
} // namespace matrixpp