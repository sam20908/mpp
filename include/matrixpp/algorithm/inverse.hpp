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

#include "../detail/matrix_base.hpp"
#include "../detail/utility.hpp"
#include "../utility/square.hpp"
#include "determinant.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <ranges>

namespace matrixpp
{
	namespace detail
	{
		template<typename Precision>
		inline void inv_impl(auto& result,
			auto& data,
			std::size_t rows,
			std::size_t columns,
			Precision determinant) // @TODO: ISSUE #20
		{
			if (rows == 0)
			{
				result[0] = static_cast<Precision>(1);
			}
			else if (rows == 1)
			{
				result[0] = static_cast<Precision>(1) / static_cast<Precision>(data[0]);
			}
			else if (rows == 2)
			{
				auto element_1 = static_cast<Precision>(data[3]);
				auto element_2 = static_cast<Precision>(data[1]) * -1;
				auto element_3 = static_cast<Precision>(data[2]) * -1;
				auto element_4 = static_cast<Precision>(data[0]);

				result[0] = element_1;
				result[1] = element_2;
				result[2] = element_3;
				result[3] = element_4;

				auto multiplier = static_cast<Precision>(1) / determinant;

				std::ranges::transform(result, result.begin(), std::bind_front(std::multiplies<>{}, multiplier));
			}
			else
			{
				/**
				* The swapping and rotating logic works a lot like `determinant_impl`'s
				* logic, except that we put the minor matrix at the top left corner and
				* call `determinant_impl` to calucate the determinant of that minor matrix.
				* We do this because We ignore the values on the current row and column
				*
				* We use the same way to rotate and swap the columns, except that we do it
				* to the rows as well
				*/

				using diff_t = typename std::decay_t<decltype(data)>::difference_type;

				// First step: Compute matrix of minors

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					if (row_index == static_cast<std::size_t>(0))
					{
						auto second_row_first_element_it = std::next(data.begin(), static_cast<diff_t>(columns));

						std::ranges::rotate(data, second_row_first_element_it);
					}
					else
					{
						auto row_n_begin_index    = (row_index - 1) * columns;
						auto last_row_begin_index = (columns - 1) * columns;

						auto row_n_begin    = std::next(data.begin(), static_cast<diff_t>(row_n_begin_index));
						auto last_row_begin = std::next(data.begin(), static_cast<diff_t>(last_row_begin_index));

						auto row_n    = std::views::counted(row_n_begin, static_cast<diff_t>(columns));
						auto last_row = std::views::counted(last_row_begin, static_cast<diff_t>(columns));

						std::ranges::swap_ranges(row_n, last_row);
					}

					for (auto column_index = std::size_t{ 0 }; column_index < columns; ++column_index)
					{
						if (column_index == 0)
						{
							for (auto minor_row_index = std::size_t{ 0 }; minor_row_index < rows - 1; ++minor_row_index)
							{
								auto minor_row_begin_index = idx_2d_to_1d(columns, minor_row_index, 0);

								auto minor_row_begin =
									std::next(data.begin(), static_cast<diff_t>(minor_row_begin_index));
								auto minor_row = std::views::counted(minor_row_begin, static_cast<diff_t>(columns));
								auto minor_row_second_element = std::next(minor_row_begin, 1);

								std::ranges::rotate(minor_row, minor_row_second_element);
							}
						}
						else
						{
							for (auto minor_row_index = std::size_t{ 0 }; minor_row_index < rows - 1; ++minor_row_index)
							{
								auto minor_row_begin_index = idx_2d_to_1d(columns, minor_row_index, 0);

								auto minor_row_begin =
									std::next(data.begin(), static_cast<diff_t>(minor_row_begin_index));
								auto minor_left_element_it =
									std::next(minor_row_begin, static_cast<diff_t>(column_index - 1));
								auto minor_right_element_it =
									std::next(minor_row_begin, static_cast<diff_t>(columns - 1));

								std::iter_swap(minor_left_element_it, minor_right_element_it);
							}
						}

						auto minor_determinant = det_impl<Precision>(data, rows, columns, 0, columns - 2);
						auto result_index      = idx_2d_to_1d(columns, row_index, column_index);

						result[result_index] = minor_determinant;
					}
				}

				// Second step: Apply a "checkerboard" pattern to the matrix of minors

				std::ranges::for_each(result, [sign = static_cast<Precision>(1)](auto& element) mutable {
					element *= sign;
					sign *= -1;
				});

				// Third step: Transpose the matrix of cofactors to its adjugate

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					for (auto column_index = std::size_t{ 0 }; column_index < row_index + 1; ++column_index)
					{
						if (row_index == column_index)
						{
							continue;
						}

						auto upper_element_index = idx_2d_to_1d(columns, row_index, column_index);
						auto lower_element_index = idx_2d_to_1d(columns, column_index, row_index);

						auto upper_element_it = std::next(result.begin(), static_cast<diff_t>(upper_element_index));
						auto lower_element_it = std::next(result.begin(), static_cast<diff_t>(lower_element_index));

						std::iter_swap(upper_element_it, lower_element_it);
					}
				}

				// Final step: Multiply by 1 / determinant of the original matrix

				// We already obtained the determinant when we calculated the matrix of
				// cofactors

				auto determinant = static_cast<Precision>(0);

				for (auto row_index = std::size_t{ 0 }; row_index < rows; ++row_index)
				{
					// The cofactors has been flipped by the previous step
					auto cofactor_index = idx_2d_to_1d(columns, row_index, 0);

					determinant += result[cofactor_index] * static_cast<Precision>(data[row_index]);
				}

				auto multiplier = static_cast<Precision>(1) / determinant;

				std::ranges::transform(result, result.begin(), std::bind_front(std::multiplies<>{}, multiplier));
			}
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline decltype(auto) inv_func(
			const matrix<Value, RowsExtent, ColumnsExtent>& obj) // @TODO: ISSUE #20
		{
			if (!square(obj))
			{
				throw std::runtime_error("Inverse of a non-square matrix doesn't exist!");
			}

			auto rows     = obj.rows();
			auto cols     = obj.columns();
			auto buf_copy = obj.buffer();

			using inverse_matrix_t = matrix<To, RowsExtent, ColumnsExtent>;
			auto inv               = inverse_matrix_t{};
			auto det               = detail::det_impl<To>(buf_copy, rows, cols, 0, cols - 1);

			// Avoid using singular here because that'd cause another determinant calculation
			if (det == static_cast<To>(0))
			{
				throw std::runtime_error("Inverse of a singular matrix doesn't exist!");
			}

			auto inv_buf = typename inverse_matrix_t::buffer_type{};

			// Allocate 1x1 for 0x0 matrix, because inverse of 0x0 matrix is 1
			auto inv_buf_nxn = rows > 0 ? rows : std::size_t{ 1 };
			detail::allocate_1d_buf_if_vector(inv_buf, inv_buf_nxn, inv_buf_nxn);

			detail::inv_impl<To>(inv_buf, buf_copy, rows, cols, det);
			detail::init_matrix_base_with_1d_rng(inv, std::move(inv_buf), inv_buf_nxn, inv_buf_nxn);

			return inv;
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
			return tag_invoke(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto inverse = inverse_t{};
} // namespace matrixpp