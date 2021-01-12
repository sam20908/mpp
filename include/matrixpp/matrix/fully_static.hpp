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
#include "matrixpp/detail/utility.hpp"

namespace matrixpp
{
	template<detail::arithmetic Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	class matrix :
		public detail::matrix_base<std::array<Value, RowsExtent * ColumnsExtent>, Value, RowsExtent, ColumnsExtent>
	{
		static_assert(!detail::dimension_not_zero_and_non_zero(RowsExtent, ColumnsExtent),
			"Cannot have one side being zero and other side being non-zero!");

		using base =
			detail::matrix_base<std::array<Value, RowsExtent * ColumnsExtent>, Value, RowsExtent, ColumnsExtent>;

	public:
		using base::base;

		constexpr explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d)
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (rows != RowsExtent || cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's dimensions does not match the provided extents!");
			}

			base::init_buf_2d_static(init_2d, rows, cols);
		}

		constexpr explicit matrix(detail::range_2d_with_type<Value> auto&& rng_2d)
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (rows != RowsExtent || cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's dimensions does not match the provided extents!");
			}

			using decayed_rng_2d_t = std::decay_t<decltype(rng_2d)>;
			base::init_buf_2d_static(std::forward<decayed_rng_2d_t>(rng_2d), rows, cols);
		}

		constexpr explicit matrix(const std::array<std::array<Value, ColumnsExtent>, RowsExtent>& arr_2d)
		{
			// We can ignore checking for same dimensions because the dimensions of the parameter uses the
			// extent template parameters, which means they'll always be the same

			base::init_buf_2d_static(arr_2d, RowsExtent, ColumnsExtent);
		}

		template<typename Expr>
		constexpr explicit matrix(const detail::expr_base<Expr, Value, RowsExtent, ColumnsExtent>& expr)
		{
			// Overload which avoids dimension checking because the expression object's extents
			// are constrained to be the same extent

			base::_rows = expr.rows();
			base::_cols = expr.columns();
			auto idx    = std::size_t{ 0 };

			for (auto row = std::size_t{ 0 }; row < base::_rows; ++row)
			{
				for (auto col = std::size_t{ 0 }; col < base::_cols; ++col)
				{
					base::_buf[idx++] = expr(row, col);
				}
			}
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		constexpr explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr)
		{
			if (RowsExtent != expr.rows() || ColumnsExtent != expr.columns())
			{
				throw std::runtime_error("Dimensions of expression object doesn't match provided extents!");
			}

			base::_rows = expr.rows();
			base::_cols = expr.columns();
			auto idx    = std::size_t{ 0 };

			for (auto row = std::size_t{ 0 }; row < base::_rows; ++row)
			{
				for (auto col = std::size_t{ 0 }; col < base::_cols; ++col)
				{
					base::_buf[idx++] = expr(row, col);
				}
			}
		}

		constexpr explicit matrix(Value value)
		{
			std::ranges::fill(base::_buf, value);
		}
	};
} // namespace matrixpp