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

#include "../detail/matrix_base.hpp"
#include "../detail/matrix_def.hpp"

namespace matrixpp
{
	template<detail::arithmetic Value>
	class matrix<Value, std::dynamic_extent, std::dynamic_extent> :
		public detail::matrix_base<std::vector<Value>, Value, std::dynamic_extent, std::dynamic_extent>
	{
		using base = detail::matrix_base<std::vector<Value>, Value, std::dynamic_extent, std::dynamic_extent>;

	public:
		using base::base;

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (detail::dimension_not_zero_and_non_zero(rows, cols))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			base::init_buf_2d_dynamic(init_2d, rows, cols);
		}

		explicit matrix(detail::range_2d_with_type<Value> auto&& rng_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (detail::dimension_not_zero_and_non_zero(rows, cols))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			using decayed_rng_2d_t = std::decay_t<decltype(rng_2d)>;
			base::init_buf_2d_dynamic(std::forward<decayed_rng_2d_t>(rng_2d), rows, cols);
		}

		template<typename Expr>
		explicit matrix(
			const detail::expr_base<Expr, Value, std::dynamic_extent, std::dynamic_extent>& expr) // @TODO: ISSUE #20
		{
			base::_rows = expr.rows();
			base::_cols = expr.columns();

			for (auto row = std::size_t{ 0 }; row < base::_rows; ++row)
			{
				for (auto col = std::size_t{ 0 }; col < base::_cols; ++col)
				{
					base::_buf.push_back(expr(row, col));
				}
			}
		}

		matrix(std::size_t rows, std::size_t columns, Value value) // @TODO: ISSUE #20
		{
			base::_rows = rows;
			base::_cols = columns;

			detail::allocate_1d_buf_if_vector(base::_buf, rows, columns);
			std::ranges::fill(base::_buf, value);
		}
	};
} // namespace matrixpp