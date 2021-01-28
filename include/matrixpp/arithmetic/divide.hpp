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
#include "../matrix.hpp"

#include <algorithm>
#include <cstddef>

namespace matrixpp
{
	namespace detail
	{
		using div_op_t = decltype([](auto&& lhs, auto&& rhs, std::size_t row_index, std::size_t column_index) {
			return lhs(row_index, column_index) / rhs;
		});
	} // namespace detail

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] decltype(auto) operator/(const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj,
		Value constant)
	{
		using obj_type = detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>;

		return detail::expr_binary_constant_op<RowsExtent, ColumnsExtent, obj_type, Value, detail::div_op_t>{ obj,
			constant,
			obj.rows(),
			obj.columns() };
	}

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] decltype(auto) operator/(Value constant,
		const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj)
	{
		using obj_type = detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>;

		return detail::expr_binary_constant_op<RowsExtent, ColumnsExtent, obj_type, Value, detail::div_op_t>{ obj,
			constant,
			obj.rows(),
			obj.columns() };
	}

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	inline decltype(auto) operator/=(matrix<Value, RowsExtent, ColumnsExtent>& obj,
		Value constant) // @TODO: ISSUE #20
	{
		// Can't use bind_front here because we want elem / constant, not constant / elem
		std::ranges::transform(obj, obj.begin(), [constant](auto elem) {
			return elem / constant;
		});

		return obj;
	}
} // namespace matrixpp
