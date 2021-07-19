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
#include <mpp/detail/expr/expr_binary_val_op.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/mat.hpp>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		inline constexpr auto mul_val_op =
			[](const auto& a, const auto& b, std::size_t row, std::size_t col) noexcept -> decltype(a(row, col) * b) {
			return a(row, col) * b;
		};

		inline constexpr auto mul_op = [](const auto& a, const auto& b, std::size_t row, std::size_t col) noexcept ->
			typename std::remove_cvref_t<decltype(a)>::value_type {
				using value_type = typename std::remove_cvref_t<decltype(a)>::value_type;

				const auto a_cols = a.cols();
				auto res          = value_type{};

				for (auto idx = std::size_t{}; idx < a_cols; ++idx)
				{
					res += a(row, idx) * b(idx, col);
				}

				return res;
			};
	} // namespace detail

	template<typename Base, typename Val, std::size_t Rows, std::size_t Cols>
	[[nodiscard]] inline auto operator*(const detail::expr_base<Base, Val, Rows, Cols>& obj, Val val) noexcept
		-> detail::expr_binary_val_op<Rows,
			Cols,
			detail::expr_base<Base, Val, Rows, Cols>,
			Val,
			decltype(detail::mul_val_op)> // @TODO: ISSUE #20
	{
		return { obj, val, obj.rows(), obj.cols(), detail::mul_val_op };
	}

	template<typename Base, typename Val, std::size_t Rows, std::size_t Cols>
	[[nodiscard]] inline auto operator*(Val val, const detail::expr_base<Base, Val, Rows, Cols>& obj)
		-> detail::expr_binary_val_op<Rows,
			Cols,
			detail::expr_base<Base, Val, Rows, Cols>,
			Val,
			decltype(detail::mul_val_op)> // @TODO: ISSUE #20
	{
		return { obj, val, obj.rows(), obj.cols(), detail::mul_val_op };
	}

	template<typename ABase,
		typename BBase,
		typename Val,
		std::size_t ARows,
		std::size_t ACols,
		std::size_t BRows,
		std::size_t BCols>
	[[nodiscard]] inline auto operator*(const detail::expr_base<ABase, Val, ARows, ACols>& a,
		const detail::expr_base<BBase, Val, BRows, BCols>& b) -> detail::expr_binary_op<ARows,
		BCols,
		detail::expr_base<ABase, Val, ARows, ACols>,
		detail::expr_base<BBase, Val, BRows, BCols>,
		decltype(detail::mul_op)> // @TODO: ISSUE #20
	{
		return { a, b, a.rows(), b.cols(), detail::mul_op };
	}

	template<typename Val, std::size_t Rows, std::size_t Cols>
	inline auto operator*=(mat<Val, Rows, Cols>& obj, Val val) -> mat<Val, Rows, Cols>& // @TODO: ISSUE #20
	{
		std::ranges::transform(obj, obj.begin(), std::bind_front(std::multiplies<>{}, val));

		return obj;
	}
} // namespace mpp
