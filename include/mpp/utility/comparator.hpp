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
	namespace detail
	{
		template<typename T>
		constexpr auto constexpr_abs(T t) -> T
		{
			if (std::is_constant_evaluated())
			{
				return t < 0 ? -t : t;
			}

			return std::abs(t);
		}
	} // namespace detail

	// Implement this as a lambda to avoid the need of LIFTING this for passing to parameters
	inline constexpr auto compare_three_way_equivalent = []<typename T>(const T& left,
															 const T& right) -> std::compare_three_way_result_t<T, T> {
		if constexpr (std::is_floating_point_v<T>)
		{
			// @FIXME: Implement our own constexpr version of abs because std::abs is not constexpr
			// @FIXME: Use adaptive epsilon (#163)
			const auto is_equivalent = detail::constexpr_abs(left - right) < std::numeric_limits<T>::epsilon();

			if (is_equivalent)
			{
				// Floating point spaceship is guaranteed to be std::partial_ordering [expr.spaceship#4.3], so we use
				// the same ordering type to indicate equivalence
				return std::partial_ordering::equivalent;
			}

			return left <=> right;
		}
		else
		{
			return left <=> right;
		}
	};
} // namespace mpp