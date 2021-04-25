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
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <utility>

namespace mpp
{
	namespace detail
	{
		template<typename To, std::size_t RowsExtent, std::size_t ColumnsExtent>
		inline auto forward_subst_unchecked(const auto& a, const auto& b)
		{
			const auto rows = a.rows();

			using x_matrix_t = mpp::matrix<default_floating_type, RowsExtent, ColumnsExtent>;
			using x_buffer_t = typename x_matrix_t::buffer_type;
			auto x_buffer    = x_buffer_t{};

			constexpr auto x_is_vector = is_vector<x_buffer_t>::value;

			reserve_buffer_if_vector(x_buffer, rows, 1);

			/**
			 * Implementation of forward substitution from
			 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-forward-backward-substitution/
			 */
			for (auto row = std::size_t{}; row < rows; ++row)
			{
				auto result = static_cast<default_floating_type>(b(row, 0));

				for (auto column = std::size_t{}; column < row; ++column)
				{
					result -= a(row, column) * x_buffer[column];
				}

				result /= a(row, row);

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

		template<typename To, std::size_t RowsExtent, std::size_t ColumnsExtent, bool Check>
		inline auto forward_subst_matrix(const auto& a, const auto& b)
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

			return mpp::matrix<To, RowsExtent, ColumnsExtent>{ a.rows(),
				1,
				forward_subst_unchecked<To, RowsExtent, ColumnsExtent>(a, b),
				mpp::unsafe };
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
			typename BAllocator>
		friend inline auto tag_invoke(forward_substitution_t,
			std::type_identity<To>,
			const mpp::matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const mpp::matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b) // @TODO: ISSUE #20
		{
			// @TODO: Figure out the constraint on To
			return detail::forward_subst_matrix<To,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				detail::configuration_use_safe>(a, b);
		}

		template<typename To,
			typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator>
		friend inline auto tag_invoke(forward_substitution_t,
			std::type_identity<To>,
			const mpp::matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const mpp::matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			mpp::unsafe_tag) // @TODO: ISSUE #20
		{
			// @TODO: Figure out the constraint on To
			return detail::forward_subst_matrix<To,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				false>(a, b);
		}

		template<typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator>
		friend inline auto tag_invoke(forward_substitution_t,
			const mpp::matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const mpp::matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b) // @TODO: ISSUE #20
		{
			return detail::forward_subst_matrix<std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				detail::configuration_use_safe>(a, b);
		}

		template<typename AValue,
			typename BValue,
			std::size_t ARowsExtent,
			std::size_t AColumnsExtent,
			std::size_t BRowsExtent,
			std::size_t BColumnsExtent,
			typename AAllocator,
			typename BAllocator>
		friend inline auto tag_invoke(forward_substitution_t,
			const mpp::matrix<AValue, ARowsExtent, AColumnsExtent, AAllocator>& a,
			const mpp::matrix<BValue, BRowsExtent, BColumnsExtent, BAllocator>& b,
			mpp::unsafe_tag) // @TODO: ISSUE #20
		{
			return detail::forward_subst_matrix<std::common_type_t<AValue, BValue>,
				detail::prefer_static_extent(ARowsExtent, AColumnsExtent),
				BColumnsExtent,
				false>(a, b);
		}
	};

	inline constexpr auto forward_substitution = forward_substitution_t{};
} // namespace mpp
