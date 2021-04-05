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
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/detail/utility/validators.hpp>
#include <mpp/matrix.hpp>

#include <cstddef>

namespace mpp
{
	namespace detail
	{
		using sub_op_type = decltype([](auto&& left, auto&& right, std::size_t row_index, std::size_t col_index)
										 -> decltype(left(row_index, col_index) - right(row_index, col_index)) {
			return left(row_index, col_index) - right(row_index, col_index);
		});
	} // namespace detail

	template<typename LeftBase,
		typename RightBase,
		typename Value,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	[[nodiscard]] inline auto operator-(
		const detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>& right)
		-> detail::expr_binary_op<detail::prefer_static_extent(LeftRowsExtent, RightRowsExtent),
			detail::prefer_static_extent(LeftColumnsExtent, RightColumnsExtent),
			detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>,
			detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>,
			detail::sub_op_type> // @TODO: ISSUE #20
	{
		detail::validate_matrices_same_size(left, right);

		return { left, right, left.rows(), left.columns() };
	}

	template<typename Value,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	inline auto operator-=(matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const matrix<Value, RightRowsExtent, RightColumnsExtent>& right)
		-> matrix<Value, LeftRowsExtent, LeftColumnsExtent>& // @TODO: ISSUE #20
	{
		detail::validate_matrices_same_size(left, right);

		std::ranges::transform(left, right, left.begin(), std::minus{});

		return left;
	}

	template<typename Value,
		typename Expr,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	inline auto operator-=(matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<Expr, Value, RightRowsExtent, RightColumnsExtent>& right)
		-> matrix<Value, LeftRowsExtent, LeftColumnsExtent>& // @TODO: ISSUE #20
	{
		detail::validate_matrices_same_size(left, right);

		const auto rows    = left.rows();
		const auto columns = left.columns();

		for (auto row = std::size_t{ 0 }; row < rows; ++row)
		{
			for (auto col = std::size_t{ 0 }; col < columns; ++col)
			{
				left(row, col) -= right(row, col);
			}
		}

		return left;
	}
} // namespace mpp
