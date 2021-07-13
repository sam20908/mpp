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
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cassert>
#include <memory>
#include <utility>

namespace mpp
{
	namespace detail
	{
		template<typename Buf>
		inline auto forward_subst_on_buffer(const auto& a, const auto& b, std::size_t n) -> Buf // @TODO: ISSUE #20
		{
			auto x_buffer = Buf{};

			allocate_buffer_if_vector(x_buffer, n, 1, default_floating_type{});

			/**
			 * Implementation of forward substitution from
			 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-forward-backward-substitution/
			 */
			for (auto row = std::size_t{}; row < n; ++row)
			{
				auto result = static_cast<default_floating_type>(b[index_2d_to_1d(1, row, 0)]);

				for (auto column = std::size_t{}; column < row; ++column)
				{
					result -= a[index_2d_to_1d(n, row, column)] * x_buffer[column];
				}

				const auto diag_elem = static_cast<default_floating_type>(a[index_2d_to_1d(n, row, row)]);

				assert(!fp_is_zero_or_nan(diag_elem));

				result /= diag_elem;

				x_buffer[row] = result;
			}

			return x_buffer;
		}

		template<typename To>
		inline auto forward_subst_matrix(const auto& a, const auto& b) -> To // @TODO: ISSUE #20
		{
			assert(square(a));
			assert(b.columns() == 1);

			const auto rows = a.rows();

			using x_mat_t = mat_rebind_to_t<To, default_floating_type>;
			auto x_buf    = forward_subst_on_buffer<typename x_mat_t::buffer_type>(a.data(), b.data(), rows);

			return To{ rows, 1, std::move(x_buf) };
		}
	} // namespace detail

	struct forward_substitution_t : public detail::cpo_base<forward_substitution_t>
	{
		template<typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator,
			typename To = matrix<std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent>>
		requires(detail::is_matrix<To>::value) friend inline auto tag_invoke(forward_substitution_t,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::forward_subst_matrix<To>(a, b);
		}
	};

	inline constexpr auto forward_substitution = forward_substitution_t{};
} // namespace mpp
