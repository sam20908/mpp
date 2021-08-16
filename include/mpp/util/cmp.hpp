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

#include <mpp/detail/util/cpo_base.hpp>
#include <mpp/detail/util/util.hpp>
#include <mpp/mat.hpp>

#include <algorithm>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace mpp
{
	struct cmp_t : public detail::cpo_base<cmp_t>
	{
		template<typename Val, typename Val2, typename Buf, typename Buf2, typename Fn = std::compare_three_way>
		[[nodiscard]] friend inline auto
		tag_invoke(cmp_t, const mat<Val, Buf>& a, const mat<Val2, Buf2>& b, Fn fn = {}) // @TODO: ISSUE #20
			noexcept(noexcept(std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end(), fn)))
		{
			return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end(), fn);
		}
	};

	inline constexpr auto cmp = cmp_t{};

	inline constexpr auto cmp_fp = []<std::floating_point T>(T a,
									   T b) noexcept -> std::compare_three_way_result_t<T, T> {
		// @TODO: Somehow avoid branching here?
		if (detail::fp_is_eq(a, b))
		{
			return std::compare_three_way_result_t<T, T>::equivalent;
		}

		return a <=> b;
	};

	inline constexpr auto cmp_fn = []<typename T>(T a, T b) noexcept -> std::compare_three_way_result_t<T, T> {
		if constexpr (std::is_floating_point_v<T>)
		{
			return cmp_fp(a, b);
		}
		else
		{
			return a <=> b;
		}
	};

	// @TODO: This is an odd place, maybe look for somewhere else to put it?
	template<typename T, typename T2, typename Buf, typename Buf2>
	auto operator<=>(const mat<T, Buf>& a, const mat<T2, Buf2>& b) noexcept(noexcept(cmp(a, b))) -> bool
	{
		return cmp(a, b);
	}
} // namespace mpp
