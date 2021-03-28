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
#include <mpp/detail/utility/exception_messages.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cmath>

namespace mpp
{
	namespace detail
	{
		inline static constexpr auto dummy_variable = ' ';

		template<bool CheckSquare, typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] inline auto det_lu_decomp(const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> To // @TODO: ISSUE #20
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

			using lu_decomp_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent>;
			using lu_decomp_buffer_t = typename lu_decomp_matrix_t::buffer_type;

			auto u_buffer = lu_decomp_buffer_t{};

			allocate_buffer_if_vector(u_buffer, rows, columns, default_floating_type{});
			std::ranges::copy(obj, u_buffer.begin());

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U) Since det(L) is always 1, we can avoid
			// creating L entirely
			const auto det = lu_decomposition_and_compute_determinant_in_place<default_floating_type, false>(rows,
				columns,
				dummy_variable,
				u_buffer);

			// We can't directly cast because that would round down floating points
			return static_cast<To>(std::round(det));
		}
	} // namespace detail

	struct determinant_t : public detail::cpo_base<determinant_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> Value // @TODO: ISSUE #20
		{
			return detail::det_lu_decomp<detail::configuration_use_unsafe, Value>(obj);
		}

		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj,
			unsafe_tag) -> Value // @TODO: ISSUE #20
		{
			return detail::det_lu_decomp<false, Value>(obj);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj) -> To // @TODO: ISSUE #20
		{
			return detail::det_lu_decomp<detail::configuration_use_unsafe, To>(obj);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj,
			unsafe_tag) -> To // @TODO: ISSUE #20
		{
			return detail::det_lu_decomp<false, To>(obj);
		}
	};

	inline constexpr auto determinant = determinant_t{};
} // namespace mpp
