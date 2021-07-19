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
	 * Binary expression object (one of the operands is a constant, so we have to store it
	 * differently, which differs from expr_binary_op)
	 */
	template<std::size_t Rows, std::size_t Cols, typename Obj, typename Val, typename Op>
	class [[nodiscard]] expr_binary_val_op :
		public expr_base<expr_binary_val_op<Rows, Cols, Obj, Val, Op>, Val, Rows, Cols>
	{
		const Obj& obj_;
		Val val_; // Store the constant by copy to handle literals

		const Op& op_;

		// "Knowing" the size of the resulting matrix allows performing validation on expression objects
		std::size_t result_rows_;
		std::size_t result_cols_;

	public:
		using value_type = Val;

		expr_binary_val_op(const Obj& obj,
			Val val,
			std::size_t result_rows,
			std::size_t result_cols,
			const Op& op) noexcept // @TODO: ISSUE #20
			:
			obj_(obj),
			val_(val),
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
			return op_(obj_, val_, row_index, col_index);
		}
	};
} // namespace mpp::detail
