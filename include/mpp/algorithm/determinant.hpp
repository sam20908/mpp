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
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cmath>

namespace mpp
{
	namespace detail
	{
		inline static constexpr auto dummy_variable = ' ';

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

			using lu_decomp_matrix_t = matrix<default_floating_type, RowsExtent, ColumnsExtent>;
			using lu_decomp_buf_t    = typename lu_decomp_matrix_t::buffer_type;

			auto u_buf = lu_decomp_buf_t{};

			allocate_1d_buf_if_vector(u_buf, rows, cols, default_floating_type{});
			std::ranges::copy(obj, u_buf.begin());

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U) Since det(L) is always 1, we can avoid
			// creating L entirely
			const auto det = lu_decomp_common<default_floating_type, false>(rows, cols, dummy_variable, u_buf);

			// We can't directly cast because that would round down floating points
			return static_cast<To>(std::round(det));
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

	struct determinant_t : public detail::cpo_base<determinant_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> Value // @TODO: ISSUE #20
		{
			return detail::det_func<Value>(obj);
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(determinant_t,
			std::type_identity<To>,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj) -> To // @TODO: ISSUE #20
		{
			return detail::det_func<To>(obj);
		}
	};

	inline constexpr auto determinant = determinant_t{};
} // namespace mpp
