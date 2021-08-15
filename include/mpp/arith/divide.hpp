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

#include <mpp/detail/expr/expr_binary_op.hpp>
#include <mpp/detail/expr/expr_binary_val_op.hpp>
#include <mpp/detail/util/algo_impl.hpp>
#include <mpp/mat.hpp>

#include <algorithm>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		inline constexpr auto div_op = [](const auto& lhs,
										   const auto& rhs,
										   std::size_t row,
										   std::size_t col) noexcept -> decltype(lhs(row, col) / rhs) {
			return lhs(row, col) / rhs;
		};
	} // namespace detail

	template<typename Derived, typename T>
	[[nodiscard]] auto operator/(const detail::expr_base<Derived, T>& obj, T val)
		-> detail::expr_binary_val_op<detail::expr_base<Derived, T>,
			T,
			decltype(detail::div_op)> // @TODO: ISSUE #20
	{
		return { obj, val, obj.rows(), obj.cols(), detail::div_op };
	}

	template<typename Derived, typename T>
	[[nodiscard]] auto operator/(T val, const detail::expr_base<Derived, T>& obj)
		-> detail::expr_binary_val_op<detail::expr_base<Derived, T>,
			T,
			decltype(detail::div_op)> // @TODO: ISSUE #20
	{
		return { obj, val, obj.rows(), obj.cols(), detail::div_op };
	}

	template<typename T, typename Buf>
	inline auto operator/=(mat<T, Buf>& obj, T val) -> mat<T, Buf>& // @TODO: ISSUE #20
	{
		// Can't use bind_front here because we want elem / val, not val / elem
		std::ranges::transform(obj, obj.begin(), [&val](const auto& elem) {
			return elem / val;
		});

		return obj;
	}
} // namespace mpp
