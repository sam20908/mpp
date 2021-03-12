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

#include <cmath>
#include <compare>
#include <limits>
#include <type_traits>
#include <utility>

namespace mpp
{
	// Implement this as a lambda to avoid the need of LIFTING this for passing to parameters
	inline constexpr auto floating_point_compare_three_way =
		[]<typename T, typename U>(const T& left, const U& right) -> std::compare_three_way_result_t<T, U> {
		using ordering_type = std::compare_three_way_result_t<T, U>;
		using common_type   = std::common_type_t<T, U>;

		const auto left_casted  = static_cast<common_type>(left);
		const auto right_casted = static_cast<common_type>(right);

		// @FIXME: Implement our own constexpr version of abs because std::abs is not constexpr
		// @FIXME: Use adaptive epsilon (#163)
		const auto is_equivalent = std::abs(left_casted - right_casted) < std::numeric_limits<common_type>::epsilon();

		if (is_equivalent)
		{
			if constexpr (std::is_floating_point_v<common_type>)
			{
				// Floating point spaceship is guaranteed to be std::partial_ordering [expr.spaceship#4.3], so we use
				// the same ordering type to indicate equivalence
				return std::partial_ordering::equivalent;
			}

			return ordering_type::equivalent;
		}

		return left_casted <=> right_casted;
	};
} // namespace mpp