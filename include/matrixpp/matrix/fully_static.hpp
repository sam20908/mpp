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

#include <matrixpp/detail/constraints.hpp>
#include <matrixpp/detail/matrix_base.hpp>
#include <matrixpp/detail/matrix_def.hpp>

#include <initializer_list>
#include <stdexcept>
#include <utility>


namespace matrixpp
{
	template<detail::arithmetic Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	class matrix :
		public detail::matrix_base<std::array<Value, RowsExtent * ColumnsExtent>, Value, RowsExtent, ColumnsExtent>
	{
		// When the user doesn't provide any other dimension extents or the extents have partial dynamic extents, it's
		// picked up by other specializations, meaning we can avoid conditional inheritance of a base class

		static_assert(!detail::dimension_not_zero_and_non_zero(RowsExtent, ColumnsExtent),
			"Cannot have one side being zero and other side being non-zero!");

		using base =
			detail::matrix_base<std::array<Value, RowsExtent * ColumnsExtent>, Value, RowsExtent, ColumnsExtent>;

	public:
		using base::base;

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (rows != RowsExtent || cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's dimensions does not match the provided extents!");
			}

			base::init_buf_2d_static(init_2d, rows, cols);
		}

		template<detail::range_2d_with_type<Value> Range2D>
		explicit matrix(Range2D&& rng_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (rows != RowsExtent || cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's dimensions does not match the provided extents!");
			}

			base::init_buf_2d_static(std::forward<Range2D>(rng_2d), rows, cols);
		}

		explicit matrix(const std::array<std::array<Value, ColumnsExtent>, RowsExtent>& arr_2d) // @TODO: ISSUE #20
		{
			// We can ignore checking for same dimensions because the dimensions of the parameter uses the
			// extent template parameters, which means they'll always be the same

			base::init_buf_2d_static(arr_2d, RowsExtent, ColumnsExtent);
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(
			const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr) // @TODO: ISSUE #20
		{
			if constexpr (RowsExtent != ExprRowsExtent || ColumnsExtent != ExprColumnsExtent)
			{
				if (RowsExtent != expr.rows() || ColumnsExtent != expr.columns())
				{
					throw std::runtime_error("Dimensions of expression object doesn't match provided extents!");
				}
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

		explicit matrix(Value value) // @TODO: ISSUE #20
		{
			base::_rows = RowsExtent;
			base::_cols = ColumnsExtent;

			std::ranges::fill(base::_buf, value);
		}

		explicit matrix(identity_matrix_tag) // @TODO: ISSUE #20
		{
			base::init_identity(RowsExtent, ColumnsExtent);
		}
	};
} // namespace matrixpp