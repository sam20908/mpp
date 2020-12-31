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

#include "../detail/expr_binary_constant_op.hpp"
#include "../detail/expr_binary_op.hpp"
#include "../detail/utility.hpp"
#include "../matrix.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>

namespace matrixpp
{
	namespace detail
	{
		inline auto mul_constant_op = [](auto&& left, auto&& right, std::size_t row_idx, std::size_t col_idx) {
			return left(row_idx, col_idx) * right;
		};

		inline auto mul_op = [](auto&& left, auto&& right, std::size_t row_idx, std::size_t col_idx) {
			using result_t = typename std::remove_cvref_t<decltype(left)>::value_type;

			auto left_cols = left.columns();
			auto result    = result_t{ 0 };

			for (auto index = std::size_t{ 0 }; index < left_cols; ++index)
			{
				result += left(row_idx, index) * right(index, col_idx);
			}

			return result;
		};

		using mul_constant_op_type = decltype(mul_constant_op);
		using mul_op_type          = decltype(mul_op);
	} // namespace detail

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline decltype(auto) operator*(const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj,
		Value constant)
	{
		using obj_type = detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>;

		return detail::
			expr_binary_constant_op<RowsExtent, ColumnsExtent, obj_type, Value, detail::mul_constant_op_type>{ obj,
				constant,
				detail::mul_constant_op,
				obj.rows(),
				obj.columns() };
	}

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline decltype(auto) operator*(Value constant,
		const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj)
	{
		using obj_type = detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>;

		return detail::
			expr_binary_constant_op<RowsExtent, ColumnsExtent, obj_type, Value, detail::mul_constant_op_type>{ obj,
				constant,
				detail::mul_constant_op,
				obj.rows(),
				obj.columns() };
	}

	template<typename LeftBase,
		typename RightBase,
		typename Value,
		std::size_t LeftRowsExtent,
		std::size_t LeftColumnsExtent,
		std::size_t RightRowsExtent,
		std::size_t RightColumnsExtent>
	[[nodiscard]] inline decltype(auto) operator*(
		const detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>& left,
		const detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>& right)
	{
		using left_type  = detail::expr_base<LeftBase, Value, LeftRowsExtent, LeftColumnsExtent>;
		using right_type = detail::expr_base<RightBase, Value, RightRowsExtent, RightColumnsExtent>;

		detail::validate_matrices_multipliable(left, right);

		return detail::expr_binary_op<LeftRowsExtent, RightColumnsExtent, left_type, right_type, detail::mul_op_type>{
			left,
			right,
			detail::mul_op,
			left.rows(),
			right.columns()
		};
	}

	template<typename Value, std::size_t LeftRowsExtent, std::size_t LeftColumnsExtent>
	inline decltype(auto) operator*=(matrix<Value, LeftRowsExtent, LeftColumnsExtent>& obj,
		Value constant) // @TODO: ISSUE #20
	{
		std::ranges::transform(obj, obj.begin(), std::bind_front(std::multiplies<>{}, constant));

		return obj;
	}
} // namespace matrixpp