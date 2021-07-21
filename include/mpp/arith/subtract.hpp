/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyB ownership.  The ASF licenses this file
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
#include <mpp/detail/util/algo_impl.hpp>
#include <mpp/detail/util/util.hpp>
#include <mpp/mat.hpp>

#include <cstddef>

namespace mpp
{
	namespace detail
	{
		inline constexpr auto sub_op =
			[](const auto& a, const auto& b, std::size_t row, std::size_t col) noexcept -> decltype(a(row, col) -
																									b(row, col)) {
			return a(row, col) - b(row, col);
		};
	} // namespace detail

	template<typename ABase,
		typename BBase,
		typename Val,
		std::size_t ARows,
		std::size_t ACols,
		std::size_t BRows,
		std::size_t BCols>
	[[nodiscard]] inline auto operator-(const detail::expr_base<ABase, Val, ARows, ACols>& a,
		const detail::expr_base<BBase, Val, BRows, BCols>& b) noexcept -> detail::expr_binary_op<dyn,
		dyn,
		detail::expr_base<ABase, Val, ARows, ACols>,
		detail::expr_base<BBase, Val, BRows, BCols>,
		decltype(detail::sub_op)> // @TODO: ISSUE #20
	{
		return { a, b, a.rows(), a.cols(), detail::sub_op };
	}

	template<typename Val, std::size_t ARows, std::size_t ACols, std::size_t BRows, std::size_t BCols>
	inline auto operator-=(mat<Val, ARows, ACols>& a, const mat<Val, BRows, BCols>& b)
		-> mat<Val, ARows, ACols>& // @TODO: ISSUE #20
	{
		std::ranges::transform(a, b, a.begin(), std::minus{});

		return a;
	}

	template<typename Val, typename Expr, std::size_t ARows, std::size_t ACols, std::size_t BRows, std::size_t BCols>
	inline auto operator-=(mat<Val, ARows, ACols>& a, const detail::expr_base<Expr, Val, BRows, BCols>& b)
		-> mat<Val, ARows, ACols>& // @TODO: ISSUE #20
	{
		const auto rows = a.rows();
		const auto cols = a.cols();

		for (auto row = std::size_t{ 0 }; row < rows; ++row)
		{
			for (auto col = std::size_t{ 0 }; col < cols; ++col)
			{
				a(row, col) -= b(row, col);
			}
		}

		return a;
	}
} // namespace mpp
