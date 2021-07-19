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

#include <mpp/detail/expr/expr_base.hpp>

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace mpp::detail
{
	/**
	 * Binary expression object (no operand is a constant)
	 */
	template<std::size_t Rows, std::size_t Cols, typename Left, typename Right, typename Op>
	class [[nodiscard]] expr_binary_op :
		public expr_base<expr_binary_op<Rows, Cols, Left, Right, Op>, typename Left::value_type, Rows, Cols>
	{
		// Store both operands by reference to avoid copying them
		const Left& left_;
		const Right& right_;

		const Op& op_;

		// "Knowing" the size of the resulting matrix allows performing validation on expression objects
		std::size_t result_rows_;
		std::size_t result_cols_;

	public:
		using value_type = typename Left::value_type;

		expr_binary_op(const Left& a,
			const Right& b,
			std::size_t result_rows,
			std::size_t result_cols,
			const Op& op) noexcept // @TODO: ISSUE #20
			:
			left_(a),
			right_(b),
			op_(op),
			result_rows_(result_rows),
			result_cols_(result_cols)
		{
		}

		[[nodiscard]] auto rows() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return result_rows_;
		}

		[[nodiscard]] auto cols() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return result_cols_;
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) const noexcept
			-> value_type // @TODO: ISSUE #20
		{
			return op_(left_, right_, row_index, col_index);
		}
	};
} // namespace mpp::detail
