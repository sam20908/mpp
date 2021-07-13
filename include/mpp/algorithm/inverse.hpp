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

#include <mpp/algorithm/back_substitution.hpp>
#include <mpp/algorithm/forward_substitution.hpp>
#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cassert>
#include <concepts>
#include <memory>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename Mat>
		[[nodiscard]] inline auto inv_impl(const Mat& obj) -> To // @TODO: ISSUE #20
		{
			assert(square(obj));

			const auto rows    = obj.rows();
			const auto columns = obj.columns();

			using lu_buf_t = typename mat_rebind_to_t<Mat, default_floating_type>::buffer_type;

			// Handle special cases - avoid LU Decomposition
			if (rows == 0)
			{
				return To{};
			}

			auto inv_buffer = lu_buf_t{};

			// If the incoming matrix has an array as its buffer, we can just use the same type of buffer since it'll be
			// less overhead and we know it's the same size

			allocate_buffer_if_vector(inv_buffer, rows, columns, default_floating_type{});

			if (rows == 1)
			{
				const auto elem = static_cast<default_floating_type>(obj(0, 0));

				assert(!fp_is_zero_or_nan(elem));

				inv_buffer[0] = 1 / elem;
			}

			if (rows == 2)
			{
				const auto element_1 = static_cast<default_floating_type>(obj(1, 1));
				const auto element_2 = static_cast<default_floating_type>(obj(0, 1));
				const auto element_3 = static_cast<default_floating_type>(obj(1, 0));
				const auto element_4 = static_cast<default_floating_type>(obj(0, 0));

				const auto ad = element_4 * element_1;
				const auto bc = element_2 * element_3;

				const auto det = ad - bc;

				assert(!fp_is_zero_or_nan(det));

				const auto multiplier = 1 / det;

				inv_buffer[0] = multiplier * element_1;
				inv_buffer[1] = multiplier * element_2 * -1;
				inv_buffer[2] = multiplier * element_3 * -1;
				inv_buffer[3] = multiplier * element_4;
			}

			if (rows >= 3)
			{
				auto l_buffer = lu_buf_t{};
				auto u_buffer = lu_buf_t{};

				// @TODO: Should do a direct buffer copy initialization instead
				allocate_buffer_if_vector(u_buffer, rows, columns, default_floating_type{});
				std::ranges::copy(obj, u_buffer.begin());

				make_identity_buffer(l_buffer, rows, columns, default_floating_type{}, default_floating_type{ 1 });

				const auto det = lu_generic<default_floating_type, true, true>(rows, columns, l_buffer, u_buffer);

				assert(!fp_is_zero_or_nan(det));

				// Solve for x_buffer values with Ax=b where A=l_buffer and b=Column of identity matrix

				using x_buf_t = typename matrix<default_floating_type, Mat::rows_extent(), 1>::buffer_type;
				auto identity_column_buffer = x_buf_t{};

				allocate_buffer_if_vector(identity_column_buffer, rows, 1, default_floating_type{});

				for (auto row = std::size_t{}; row < rows; ++row)
				{
					// Branch-less logic to do:
					// row if row == 0
					// row - 1 if row > 0
					const auto last_column_index = row - 1 * (row != 0);

					// This forms b that corresponds to corresponding column from the identity matrix
					// e.g. with a 3x1 column vector
					// row 1 -> 1 0 0
					// row 2 -> 0 1 0
					// row 3 -> 0 0 1
					identity_column_buffer[last_column_index] = default_floating_type{};
					identity_column_buffer[row]               = default_floating_type{ 1 };

					auto l_x_buffer = forward_subst_on_buffer<x_buf_t>(l_buffer, identity_column_buffer, rows);

					// Use l_x_buffer to do back substitution to solve Ax=B with A=u_buffer and b=l_x_buffer. The
					// part_inverse_buffer now corresponds to a column of the inverse matrix

					auto part_inverse_buffer = back_subst_on_buffer<x_buf_t>(u_buffer, std::move(l_x_buffer), rows);

					for (auto column = std::size_t{}; auto&& value : part_inverse_buffer)
					{
						inv_buffer[index_2d_to_1d(columns, column++, row)] = std::move(value);
					}
				}
			}

			return To{ rows, columns, std::move(inv_buffer) };
		}
	} // namespace detail

	struct inverse_t : public detail::cpo_base<inverse_t>
	{
		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename To = matrix<Value, RowsExtent, ColumnsExtent, Allocator>>
		requires(detail::is_matrix<To>::value) [[nodiscard]] friend inline auto tag_invoke(inverse_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::inv_impl<To>(obj);
		}
	};

	inline constexpr auto inverse = inverse_t{};
} // namespace mpp
