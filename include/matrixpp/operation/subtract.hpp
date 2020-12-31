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

#include "../detail/expr_binary_op.hpp"
#include "../detail/utility.hpp"
#include "../matrix.hpp"

#include <cstddef>
#include <span>

namespace matrixpp
{
	namespace detail
	{
		inline auto sub_op = [](auto&& left, auto&& right, std::size_t row_idx, std::size_t col_idx) {
			return left(row_idx, col_idx) - right(row_idx, col_idx);
		};

		using sub_op_type = decltype(sub_op);
	} // namespace detail

	template<typename LeftBase, typename RightBase, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline decltype(auto) operator-(
		const detail::expr_base<LeftBase, Value, RowsExtent, ColumnsExtent>& left,
		const detail::expr_base<RightBase, Value, RowsExtent, ColumnsExtent>& right) // @TODO: ISSUE #20
	{
		// Subtraction of matrices with same dimension extents

		using left_type  = detail::expr_base<LeftBase, Value, RowsExtent, ColumnsExtent>;
		using right_type = detail::expr_base<RightBase, Value, RowsExtent, ColumnsExtent>;

		detail::validate_matrices_same_size(left, right);

		return detail::expr_binary_op<RowsExtent, ColumnsExtent, left_type, right_type, detail::sub_op_type>{ left,
			right,
			detail::sub_op,
			left.rows(),
			left.columns() };
	}

	template<typename LeftBase,
		typename RightBase,
		typename Value,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	[[nodiscard]] inline decltype(auto) operator-(
		const detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>& right) // @TODO: ISSUE #20
	{
		// Subtraction of matrices with different dimension extents

		using left_type  = detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>;
		using right_type = detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>;

		detail::validate_matrices_same_size(left, right);

		return detail::
			expr_binary_op<std::dynamic_extent, std::dynamic_extent, left_type, right_type, detail::sub_op_type>{ left,
				right,
				detail::sub_op,
				left.rows(),
				left.columns() };
	}

	template<typename Value,
		typename Expr,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	inline decltype(auto) operator-=(matrix<Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<Expr, Value, RightRowsExtent, RightColumnsExtent>& right) // @TODO: ISSUE #20
	{
		detail::validate_matrices_same_size(left, right);

		auto rows = left.rows();
		auto cols = left.cols();

		for (auto row = std::size_t{ 0 }; row < rows; ++row)
		{
			for (auto col = std::size_t{ 0 }; col < cols; ++col)
			{
				left(row, col) -= right(row, col);
			}
		}

		return left;
	}
} // namespace matrixpp