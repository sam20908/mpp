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
#include <mpp/detail/utility/exception_messages.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <memory>
#include <utility>

namespace mpp
{
	namespace detail
	{
		template<bool Check, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
		inline auto back_subst_on_buffer(const auto& a, const auto& b, std::size_t n) -> // @TODO: ISSUE #20
			typename matrix<default_floating_type, RowsExtent, ColumnsExtent, Allocator>::buffer_type
		{
			using x_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent, Allocator>;
			using x_buffer_t = typename x_matrix_t::buffer_type;
			auto x_buffer    = x_buffer_t{};

			// @TODO: Any way to make this utilize push_back?
			allocate_buffer_if_vector(x_buffer, n, 1, default_floating_type{});

			/**
			 * Implementation of back substitution from
			 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-back-backward-substitution/
			 */
			for (auto row = n; row > std::size_t{}; --row)
			{
				const auto row_index = row - 1;

				auto result = static_cast<default_floating_type>(b[index_2d_to_1d(1, row_index, 0)]);

				for (auto column = n - 1; column > row_index; --column)
				{
					result -= a[index_2d_to_1d(n, row_index, column)] * x_buffer[column];
				}

				const auto diag_elem = static_cast<default_floating_type>(a[index_2d_to_1d(n, row_index, row_index)]);

				if constexpr (Check)
				{
					if (fp_is_zero_or_nan(diag_elem))
					{
						throw std::runtime_error(DIAGONAL_ELEMENT_IS_ZERO);
					}
				}

				result /= diag_elem;

				x_buffer[row_index] = result;
			}

			return x_buffer;
		}

		template<bool Check, typename To, std::size_t RowsExtent, std::size_t ColumnsExtent, typename ToAllocator>
		inline auto back_subst_matrix(const auto& a, const auto& b)
			-> matrix<To, RowsExtent, ColumnsExtent, ToAllocator> // @TODO: ISSUE #20
		{
			// @FIXME: Find best message to error about a.rows() != b.rows()
			if constexpr (Check)
			{
				if (!square(a))
				{
					throw std::logic_error(MATRIX_NOT_SQUARE);
				}

				if (const auto b_cols = b.columns(); b_cols != 1)
				{
					throw std::logic_error(MATRIX_NOT_COLUMN_VECTOR);
				}
			}

			return matrix<To, RowsExtent, ColumnsExtent, ToAllocator>{ a.rows(),
				1,
				back_subst_on_buffer<Check,
					RowsExtent,
					ColumnsExtent,
					typename std::allocator_traits<ToAllocator>::template rebind_alloc<To>>(a.data(),
					b.data(),
					a.rows()),
				unsafe };
		}
	} // namespace detail

	struct back_substitution_t : public detail::cpo_base<back_substitution_t>
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
			typename ToAllocator = typename std::allocator_traits<AAllocator>::template rebind_alloc<To>>
		friend inline auto tag_invoke(back_substitution_t,
			std::type_identity<To>,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			std::type_identity<ToAllocator> = {}) -> matrix<To,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			// @TODO: Figure out the constraint on To
			return detail::back_subst_matrix<detail::configuration_use_safe,
				To,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b);
		}

		template<typename To,
			typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator,
			typename ToAllocator = typename std::allocator_traits<AAllocator>::template rebind_alloc<To>>
		friend inline auto tag_invoke(back_substitution_t,
			std::type_identity<To>,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			unsafe_tag,
			std::type_identity<ToAllocator> = {}) -> matrix<To,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			// @TODO: Figure out the constraint on To
			return detail::back_subst_matrix<false,
				To,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b);
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
		friend inline auto tag_invoke(back_substitution_t,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			std::type_identity<ToAllocator> = {}) -> matrix<std::common_type_t<AValue, BValue>,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			return detail::back_subst_matrix<detail::configuration_use_safe,
				std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b);
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
		friend inline auto tag_invoke(back_substitution_t,
			const matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			unsafe_tag,
			std::type_identity<ToAllocator> = {}) -> matrix<std::common_type_t<AValue, BValue>,
			detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
			BColumnsExtent,
			ToAllocator> // @TODO: ISSUE #20
		{
			return detail::back_subst_matrix<false,
				std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				ToAllocator>(a, b);
		}
	};

	inline constexpr auto back_substitution = back_substitution_t{};
} // namespace mpp
