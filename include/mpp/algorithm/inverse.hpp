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

#include <mpp/detail/algo_types.hpp>
#include <mpp/detail/cpo_base.hpp>
#include <mpp/detail/utility.hpp>
#include <mpp/utility/comparator.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <concepts>
#include <future>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		inline void forward_substitution(auto& l_buf, std::size_t cols) // @TODO: ISSUE #20
		{
			for (auto col = std::size_t{ 1 }; col < cols; ++col)
			{
				// Optimized version of forward-substitution which skips making diagnoal 1's because L would've already
				// had 1's along the diagonal

				for (auto row = col + 1; row < cols; ++row)
				{
					const auto factor = l_buf[idx_2d_to_1d(cols, row, row - 1)] * -1;

					for (auto col_2 = std::size_t{}; col_2 < col; ++col_2)
					{
						const auto elem_above = l_buf[idx_2d_to_1d(cols, col, col_2)];
						const auto elem_idx   = idx_2d_to_1d(cols, row, col_2);

						l_buf[elem_idx] -= factor * elem_above;
					}
				}
			}
		}

		inline void back_substitution(auto& u_buf, std::size_t cols) // @TODO: ISSUE #20
		{
			for (auto col = cols; col > 0; --col)
			{
				const auto col_idx   = col - 1;
				auto diag_elem_idx   = idx_2d_to_1d(cols, col_idx, col_idx);
				const auto diag_elem = u_buf[diag_elem_idx];

				// Diagonal element can simply be replaced with the factor
				const auto diag_factor = default_floating_type{ 1 } / diag_elem;
				u_buf[diag_elem_idx]   = diag_factor;

				// Multiply every element to the right of the diagonal element by the factor
				for (auto idx = cols - col; idx > 0; --idx)
				{
					u_buf[++diag_elem_idx] *= diag_factor;
				}

				for (auto row = col_idx; row > 0; --row)
				{
					// Use the diagonal element as the factor to compute the numbers above the pivot in the same column
					// (this works because) the augmented matrix would have zeroes above the diagonal element
					const auto row_idx            = row - 1;
					const auto elem_idx           = idx_2d_to_1d(cols, row_idx, col_idx);
					const auto elem_before_factor = u_buf[elem_idx];
					u_buf[elem_idx]               = elem_before_factor * diag_factor * -1;

					// Add the corresponding elements of the rows of the current diagonal element onto the rows above
					for (auto col_2 = cols; col_2 > col; --col_2)
					{
						const auto col_2_idx     = col_2 - 1;
						auto diag_row_idx        = idx_2d_to_1d(cols, col_idx, col_2_idx);
						const auto diag_row_elem = u_buf[diag_row_idx];

						const auto cur_elem_idx = idx_2d_to_1d(cols, row_idx, col_2_idx);
						const auto elem         = u_buf[cur_elem_idx];
						const auto factor       = elem_before_factor * -1;

						const auto new_elem = factor * diag_row_elem + elem;
						u_buf[cur_elem_idx] = new_elem;
					}
				}
			}
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto inv_lu_decomp(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			const auto rows = obj.rows();
			const auto cols = obj.columns();

			using inv_matrix_t = matrix<To, RowsExtent, ColumnsExtent>;
			auto inv_matrix    = inv_matrix_t{};

			// Handle special cases - avoid LU Decomposition
			if (rows == 0)
			{
				return inv_matrix;
			}

			auto inv_matrix_buf = typename inv_matrix_t::buffer_type{};
			allocate_1d_buf_if_vector(inv_matrix_buf, rows, cols, To{});

			using default_floating_type_ordering_type =
				std::compare_three_way_result_t<default_floating_type, default_floating_type>;

			if (rows == 1)
			{
				const auto elem = static_cast<To>(obj(0, 0));

				using to_ordering_type = std::compare_three_way_result_t<To, To>;

				if (floating_point_compare(elem, To{}) == to_ordering_type::equivalent)
				{
					throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
				}

				inv_matrix_buf[0] = To{ 1 } / elem;
			}

			auto det = default_floating_type{ 1 };

			if (rows == 2)
			{
				const auto element_1 = static_cast<default_floating_type>(obj(1, 1));
				const auto element_2 = static_cast<default_floating_type>(obj(0, 1));
				const auto element_3 = static_cast<default_floating_type>(obj(1, 0));
				const auto element_4 = static_cast<default_floating_type>(obj(0, 0));

				const auto ad = element_4 * element_1;
				const auto bc = element_2 * element_3;

				det = ad - bc;

				if (floating_point_compare(det, default_floating_type{}) ==
					default_floating_type_ordering_type::equivalent)
				{
					throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
				}

				const auto multiplier = default_floating_type{ 1 } / det;

				inv_matrix_buf[0] = static_cast<To>(multiplier * element_1);
				inv_matrix_buf[1] = static_cast<To>(multiplier * element_2 * -1);
				inv_matrix_buf[2] = static_cast<To>(multiplier * element_3 * -1);
				inv_matrix_buf[3] = static_cast<To>(multiplier * element_4);
			}

			if (rows >= 3)
			{
				using lu_decomp_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent>;
				using lu_decomp_buf_t    = typename lu_decomp_matrix_t::buffer_type;

				auto l_buf = lu_decomp_buf_t{};
				auto u_buf = lu_decomp_buf_t{};

				allocate_1d_buf_if_vector(u_buf, rows, cols, default_floating_type{});
				std::ranges::copy(obj, u_buf.begin());

				allocate_1d_buf_if_vector(l_buf, rows, cols, default_floating_type{});
				transform_1d_buf_into_identity<default_floating_type>(l_buf, rows);

				det = lu_decomp_common<default_floating_type, true>(rows, cols, l_buf, u_buf);

				if (floating_point_compare(det, default_floating_type{}) ==
					default_floating_type_ordering_type::equivalent)
				{
					throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
				}

				if (std::is_constant_evaluated())
				{
					forward_substitution(l_buf, cols);
					back_substitution(u_buf, cols);
				}
				else
				{
					// Compute inv(L) and inv(U) in parallel because they don't share data

					auto l_inv_future = std::async(std::launch::async, [&l_buf, cols]() {
						forward_substitution(l_buf, cols);
					});
					auto u_inv_future = std::async(std::launch::async, [&u_buf, cols]() {
						back_substitution(u_buf, cols);
					});

					l_inv_future.wait();
					u_inv_future.wait();
				}

				mul_square_bufs<To, default_floating_type>(inv_matrix_buf, std::move(u_buf), std::move(l_buf), rows);
			}

			init_matrix_with_1d_rng(inv_matrix, std::move(inv_matrix_buf), rows, cols);
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

	struct inverse_t : public detail::cpo_base<inverse_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(inverse_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<detail::default_floating_type, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			return detail::inv_func<detail::default_floating_type>(obj);
		}

		template<std::floating_point To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto
		tag_invoke(inverse_t, std::type_identity<To>, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			return detail::inv_func<To>(obj);
		}
	};

	inline constexpr auto inverse = inverse_t{};
} // namespace mpp
