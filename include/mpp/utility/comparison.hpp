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

#include <mpp/detail/cpo_base.hpp>
#include <mpp/detail/utility.hpp>
#include <mpp/matrix.hpp>

#include <compare>
#include <cstddef>
#include <span>
#include <type_traits>
#include <utility>

namespace mpp
{
	struct size_compare_t : public detail::cpo_base<size_compare_t>
	{
		template<typename LeftValue,
			typename RightValue,
			std::size_t LeftRowsExtent,
			std::size_t LeftColumnsExtent,
			std::size_t RightRowsExtent,
			std::size_t RightColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(size_compare_t,
			const matrix<LeftValue, LeftRowsExtent, LeftColumnsExtent>& left,
			const matrix<RightValue, RightRowsExtent, RightColumnsExtent>& right,
			bool compare_rows,
			bool compare_columns) -> std::pair<std::partial_ordering, std::partial_ordering> // @TODO: ISSUE #20
		{
			return std::pair{ compare_rows ? left.rows() <=> right.rows() : std::partial_ordering::unordered,
				compare_columns ? left.columns() <=> right.columns() : std::partial_ordering::unordered };
		}
	};

	struct elements_compare_t : public detail::cpo_base<elements_compare_t>
	{
		template<typename Value,
			std::size_t LeftRowsExtent,
			std::size_t LeftColumnsExtent,
			std::size_t RightRowsExtent,
			std::size_t RightColumnsExtent,
			typename CompareThreeway = std::compare_three_way>
		[[nodiscard]] friend inline auto tag_invoke(elements_compare_t,
			const matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
			const matrix<Value, RightRowsExtent, RightColumnsExtent>& right,
			CompareThreeway compare_three_way_fn = {}) // @TODO: ISSUE #20
		{
			const auto left_size  = left.size();
			const auto right_size = right.size();

			using ordering_type = std::compare_three_way_result_t<Value, Value>;

			if (left_size != right_size)
			{
				return ordering_type{ left_size <=> right_size };
			}

			// We can't just use <=> because of two different types of matrices to compare

			const auto end = left.cend();

			auto left_it  = left.cbegin();
			auto right_it = right.cbegin();

			for (; left_it != end; ++left_it, ++right_it)
			{
				const auto ordering = compare_three_way_fn(*left_it, *right_it);

				if (ordering != 0)
				{
					return ordering;
				}
			}

			return ordering_type::equivalent;
		}
	};

	inline constexpr auto size_compare     = size_compare_t{};
	inline constexpr auto elements_compare = elements_compare_t{};
} // namespace mpp
