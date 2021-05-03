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

#include <mpp/detail/expr/expr_binary_constant_op.hpp>
#include <mpp/detail/expr/expr_binary_op.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/validators.hpp>
#include <mpp/matrix.hpp>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		inline constexpr auto mul_constant_op =
			[](const auto& left,
				const auto& right,
				std::size_t row_index,
				std::size_t col_index) noexcept -> decltype(left(row_index, col_index) * right) {
			return left(row_index, col_index) * right;
		};

		inline constexpr auto mul_op =
			[](const auto& left, const auto& right, std::size_t row_index, std::size_t col_index) noexcept ->
			typename std::decay_t<decltype(left)>::value_type {
				using value_type = typename std::decay_t<decltype(left)>::value_type;

				const auto left_columns = left.columns();
				auto result             = value_type{};

				for (auto index = std::size_t{}; index < left_columns; ++index)
				{
					result += left(row_index, index) * right(index, col_index);
				}

				return result;
			};
	} // namespace detail

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline auto operator*(const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj,
		Value constant) noexcept -> detail::expr_binary_constant_op<RowsExtent,
		ColumnsExtent,
		detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>,
		Value,
		decltype(detail::mul_constant_op)> // @TODO: ISSUE #20
	{
		return { obj, constant, obj.rows(), obj.columns(), detail::mul_constant_op };
	}

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline auto operator*(Value constant,
		const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj)
		-> detail::expr_binary_constant_op<RowsExtent,
			ColumnsExtent,
			detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>,
			Value,
			decltype(detail::mul_constant_op)> // @TODO: ISSUE #20
	{
		return { obj, constant, obj.rows(), obj.columns(), detail::mul_constant_op };
	}

	template<typename LeftBase,
		typename RightBase,
		typename Value,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	[[nodiscard]] inline auto operator*(
		const detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>& right)
		-> detail::expr_binary_op<LeftRowsExtent,
			RightColumnsExtent,
			detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>,
			detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>,
			decltype(detail::mul_op)> // @TODO: ISSUE #20
	{
		detail::validate_matrices_multipliable(left, right);

		return { left, right, left.rows(), right.columns(), detail::mul_op };
	}

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	inline auto operator*=(matrix<Value, RowsExtent, ColumnsExtent>& obj, Value constant)
		-> matrix<Value, RowsExtent, ColumnsExtent>& // @TODO: ISSUE #20
	{
		std::ranges::transform(obj, obj.begin(), std::bind_front(std::multiplies<>{}, constant));

		return obj;
	}
} // namespace mpp
