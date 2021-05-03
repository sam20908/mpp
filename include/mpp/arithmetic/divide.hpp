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

namespace mpp
{
	namespace detail
	{
		inline constexpr auto div_op = [](const auto& lhs,
										   const auto& rhs,
										   std::size_t row_index,
										   std::size_t column_index) noexcept -> decltype(lhs(row_index, column_index) /
																						  rhs) {
			return lhs(row_index, column_index) / rhs;
		};
	} // namespace detail

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] auto operator/(const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj, Value constant)
		-> detail::expr_binary_constant_op<RowsExtent,
			ColumnsExtent,
			detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>,
			Value,
			decltype(detail::div_op)> // @TODO: ISSUE #20
	{
		return { obj, constant, obj.rows(), obj.columns(), detail::div_op };
	}

	template<typename Base, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] auto operator/(Value constant, const detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>& obj)
		-> detail::expr_binary_constant_op<RowsExtent,
			ColumnsExtent,
			detail::expr_base<Base, Value, RowsExtent, ColumnsExtent>,
			Value,
			decltype(detail::div_op)> // @TODO: ISSUE #20
	{
		return { obj, constant, obj.rows(), obj.columns(), detail::div_op };
	}

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	inline auto operator/=(matrix<Value, RowsExtent, ColumnsExtent>& obj, Value constant)
		-> matrix<Value, RowsExtent, ColumnsExtent>& // @TODO: ISSUE #20
	{
		// Can't use bind_front here because we want elem / constant, not constant / elem
		std::ranges::transform(obj, obj.begin(), [&constant](const auto& elem) {
			return elem / constant;
		});

		return obj;
	}
} // namespace mpp
