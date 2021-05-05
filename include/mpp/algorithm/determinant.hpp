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
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/detail/utility/exception_messages.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cmath>
#include <memory>

namespace mpp
{
	namespace detail
	{
		inline static constexpr auto dummy_variable = ' ';

		template<bool CheckSquare,
			typename To,
			typename LUAllocator,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator>
		[[nodiscard]] inline auto det_impl(const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			const auto&... alloc_args) -> To // @TODO: ISSUE #20
		{
			if constexpr (CheckSquare)
			{
				if (!square(obj))
				{
					throw std::runtime_error(MATRIX_NOT_SQUARE);
				}
			}

			const auto rows    = obj.rows();
			const auto columns = obj.columns();

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

			using lu_decomp_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent, LUAllocator>;
			using lu_decomp_buffer_t = typename lu_decomp_matrix_t::buffer_type;

			auto u_buffer = lu_decomp_buffer_t{ alloc_args... };

			allocate_buffer_if_vector(u_buffer, rows, columns, default_floating_type{});
			std::ranges::copy(obj, u_buffer.begin());

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U) Since det(L) is always 1, we can avoid
			// creating L entirely
			const auto det = lu_generic<default_floating_type, false, true>(rows, columns, dummy_variable, u_buffer);

			// We can't directly cast because that would round down floating points
			// @TODO: Analyze if other algorithms that uses determinant need this treatment
			return static_cast<To>(det);
		}
	} // namespace detail

	struct determinant_t : public detail::cpo_base<determinant_t>
	{
		// @TODO: Customizing LUAllocator is confusing, find something better

		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			std::type_identity<LUAllocator> = {}) -> Value // @TODO: ISSUE #20
		{
			return detail::det_impl<detail::configuration_use_safe, Value, LUAllocator>(obj);
		}

		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			const LUAllocator& lu_alloc) -> Value // @TODO: ISSUE #20
		{
			return detail::det_impl<detail::configuration_use_safe, Value, LUAllocator>(obj, lu_alloc);
		}

		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			unsafe_tag,
			std::type_identity<LUAllocator> = {}) -> Value // @TODO: ISSUE #20
		{
			return detail::det_impl<false, Value, LUAllocator>(obj);
		}

		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			unsafe_tag,
			const LUAllocator& lu_alloc) -> Value // @TODO: ISSUE #20
		{
			return detail::det_impl<false, Value, LUAllocator>(obj, lu_alloc);
		}

		template<typename To,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			std::type_identity<LUAllocator> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::det_impl<detail::configuration_use_safe, To, LUAllocator>(obj);
		}

		template<typename To,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			const LUAllocator& lu_alloc) -> To // @TODO: ISSUE #20
		{
			return detail::det_impl<detail::configuration_use_safe, To, LUAllocator>(obj, lu_alloc);
		}

		template<typename To,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			unsafe_tag,
			std::type_identity<LUAllocator> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::det_impl<false, To, LUAllocator>(obj);
		}

		template<typename To,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename LUAllocator =
				typename std::allocator_traits<Allocator>::template rebind_alloc<detail::default_floating_type>>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			unsafe_tag,
			const LUAllocator& lu_alloc) -> To // @TODO: ISSUE #20
		{
			return detail::det_impl<false, To, LUAllocator>(obj, lu_alloc);
		}
	};

	inline constexpr auto determinant = determinant_t{};
} // namespace mpp
