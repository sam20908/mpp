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

#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/matrix.hpp>

#include <algorithm>
#include <cmath>
#include <compare>
#include <cstddef>
#include <type_traits>

namespace mpp
{
	struct size_compare_t : public detail::cpo_base<size_compare_t>
	{
		template<typename LeftValue,
			typename RightValue,
			std::size_t LeftRowsExtent,
			std::size_t LeftColumnsExtent,
			std::size_t RightRowsExtent,
			std::size_t RightColumnsExtent,
			typename LeftAllocator,
			typename RightAllocator>
		[[nodiscard]] friend inline auto tag_invoke(size_compare_t,
			const matrix<LeftValue, LeftRowsExtent, LeftColumnsExtent, LeftAllocator>& left,
			const matrix<RightValue, RightRowsExtent, RightColumnsExtent, RightAllocator>& right,
			bool compare_rows,
			bool compare_columns) noexcept(noexcept(std::pair{
			compare_rows ? left.rows() <=> right.rows() : std::partial_ordering::unordered,
			compare_columns ? left.columns() <=> right.columns() : std::partial_ordering::unordered }))
			-> std::pair<std::partial_ordering, std::partial_ordering> // @TODO: ISSUE #20
		{
			return std::pair{ compare_rows ? left.rows() <=> right.rows() : std::partial_ordering::unordered,
				compare_columns ? left.columns() <=> right.columns() : std::partial_ordering::unordered };
		}
	};

	struct elements_compare_t : public detail::cpo_base<elements_compare_t>
	{
		template<typename LeftValue,
			typename RightValue,
			std::size_t LeftRowsExtent,
			std::size_t LeftColumnsExtent,
			std::size_t RightRowsExtent,
			std::size_t RightColumnsExtent,
			typename LeftAllocator,
			typename RightAllocator,
			typename CompareThreeway = std::compare_three_way>
		[[nodiscard]] friend inline auto tag_invoke(elements_compare_t,
			const matrix<LeftValue, LeftRowsExtent, LeftColumnsExtent, LeftAllocator>& left,
			const matrix<RightValue, RightRowsExtent, RightColumnsExtent, RightAllocator>& right,
			CompareThreeway compare_three_way_fn = {})
#ifndef __clang__ // Testing if Clang crashes with this noexcept specification
			noexcept(noexcept(std::lexicographical_compare_three_way(left.begin(),
				left.end(),
				right.begin(),
				right.end(),
				compare_three_way_fn)))
#endif // @TODO: ISSUE #20
		{
			return std::lexicographical_compare_three_way(left.begin(),
				left.end(),
				right.begin(),
				right.end(),
				compare_three_way_fn);
		}
	};

	inline constexpr auto size_compare     = size_compare_t{};
	inline constexpr auto elements_compare = elements_compare_t{};

	/**
	 * Comparators
	 */

	namespace detail
	{
		template<typename T>
		constexpr auto constexpr_abs(T t) noexcept -> T
		{
			if (std::is_constant_evaluated())
			{
				return t < 0 ? -t : t;
			}

			return std::abs(t);
		}
	} // namespace detail

	inline constexpr auto floating_point_compare =
		[]<typename T, typename U>(const T& left, const U& right) noexcept -> std::compare_three_way_result_t<T, U> {
		using common_type   = std::common_type_t<T, U>;
		using ordering_type = std::compare_three_way_result_t<common_type, common_type>;

		const auto left_casted  = static_cast<common_type>(left);
		const auto right_casted = static_cast<common_type>(right);

		const auto is_equivalent =
			detail::constexpr_abs(left_casted - right_casted) < std::numeric_limits<common_type>::epsilon();

		// @TODO: Somehow avoid branching here?
		if (is_equivalent)
		{
			return ordering_type::equivalent;
		}

		return left_casted <=> right_casted;
	};
} // namespace mpp
