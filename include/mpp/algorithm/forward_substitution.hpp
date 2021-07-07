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
		template<std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator, typename... Args>
		inline auto forward_subst_on_buffer(const auto& a, const auto& b, std::size_t n) -> // @TODO: ISSUE #20
			typename matrix<default_floating_type, RowsExtent, ColumnsExtent, Allocator>::buffer_type
		{
			using x_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent, Allocator>;
			using x_buffer_t = typename x_matrix_t::buffer_type;
			auto x_buffer    = x_buffer_t{};

			constexpr auto x_is_vector = is_vector<x_buffer_t>::value;

			reserve_buffer_if_vector(x_buffer, n, 1);

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

				if constexpr (x_is_vector)
				{
					x_buffer.push_back(result);
				}
				else
				{
					x_buffer[row] = result;
				}
			}

			return x_buffer;
		}

		template<typename To, std::size_t RowsExtent, std::size_t ColumnsExtent, typename ToAllocator, typename... Args>
		inline auto forward_subst_matrix(const auto& a, const auto& b, [[maybe_unused]] const Args&... alloc_args)
			-> matrix<To, RowsExtent, ColumnsExtent, ToAllocator> // @TODO: ISSUE #20
		{
			assert(square(a));
			assert(b.columns() == 1);

			const auto rows = a.rows();

			using x_mat_t = matrix<To, RowsExtent, ColumnsExtent, ToAllocator>;
			auto&& x_buf  = forward_subst_on_buffer<RowsExtent,
                ColumnsExtent,
                typename std::allocator_traits<ToAllocator>::template rebind_alloc<default_floating_type>>(a.data(),
                b.data(),
                rows);

			return [&]() {
				if constexpr (any_extent_is_dynamic(RowsExtent, ColumnsExtent))
				{
					return x_mat_t{ rows, 1, std::move(x_buf), alloc_args... };
				}
				else
				{
					return x_mat_t{ rows, 1, std::move(x_buf) };
				}
			}();
		}
	} // namespace detail

	struct forward_substitution_t : public detail::cpo_base<forward_substitution_t>
	{
		template<typename To,
			typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator,
			typename ToAllocator =
				typename std::allocator_traits<AAllocator>::template rebind_alloc<std::common_type_t<AValue, BValue>>>
		friend inline auto tag_invoke(forward_substitution_t,
			std::type_identity<To>,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			const ToAllocator& to_alloc = ToAllocator{}) -> matrix<To,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			// @TODO: Figure out the constraint on To
			return detail::forward_subst_matrix<To,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b, to_alloc);
		}

		template<typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator,
			typename ToAllocator =
				typename std::allocator_traits<AAllocator>::template rebind_alloc<std::common_type_t<AValue, BValue>>>
		friend inline auto tag_invoke(forward_substitution_t,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			const ToAllocator& to_alloc = ToAllocator{}) -> matrix<std::common_type_t<AValue, BValue>,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			return detail::forward_subst_matrix<std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b, to_alloc);
		}
	};

	inline constexpr auto forward_substitution = forward_substitution_t{};
} // namespace mpp
