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

#include <mpp/detail/expr_base.hpp>

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace mpp::detail
{
	/**
	 * Binary expression object (no operand is a constant)
	 */
	template<std::size_t RowsExtent, std::size_t ColumnsExtent, typename Left, typename Right, typename Op>
	class [[nodiscard]] expr_binary_op :
		public expr_base<expr_binary_op<RowsExtent, ColumnsExtent, Left, Right, Op>,
			typename Left::value_type,
			RowsExtent,
			ColumnsExtent>
	{
		// Store both operands by reference to avoid copying them
		const Left& _left;
		const Right& _right;

		Op _op{};

		// "Knowing" the size of the resulting matrix allows performing validation on expression objects
		std::size_t _result_rows;
		std::size_t _result_columns;

	public:
		using value_type = typename Left::value_type;

		expr_binary_op(const Left& left,
			const Right& right,
			std::size_t result_rows,
			std::size_t result_columns) // @TODO: ISSUE #20
			:
			_left(left),
			_right(right),
			_result_rows(result_rows),
			_result_columns(result_columns)
		{
		}

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return _result_rows;
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return _result_columns;
		}

		[[nodiscard]] auto at(std::size_t row_index, std::size_t col_index) const -> value_type // @TODO: ISSUE #20
		{
			if (row_index >= _result_rows || col_index >= _result_columns)
			{
				throw std::out_of_range("Access out of range!");
			}

			return operator()(row_index, col_index);
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) const -> value_type // @TODO: ISSUE #20
		{
			return _op(_left, _right, row_index, col_index);
		}
	};
} // namespace mpp::detail