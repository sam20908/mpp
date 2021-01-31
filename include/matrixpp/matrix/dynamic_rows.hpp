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

#include <initializer_list>
#include <matrixpp/detail/constraints.hpp>
#include <matrixpp/detail/matrix_base.hpp>
#include <matrixpp/detail/matrix_def.hpp>
#include <stdexcept>
#include <utility>

namespace matrixpp
{
	template<detail::arithmetic Value, std::size_t ColumnsExtent>
	class matrix<Value, std::dynamic_extent, ColumnsExtent> :
		public detail::matrix_base<std::vector<Value>, Value, std::dynamic_extent, ColumnsExtent>
	{
		using base = detail::matrix_base<std::vector<Value>, Value, std::dynamic_extent, ColumnsExtent>;

	public:
		using base::base;

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's columns does not match the provided column extent!");
			}

			if (detail::dimension_not_zero_and_non_zero(rows, ColumnsExtent))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			base::init_buf_2d_dynamic(init_2d, rows, ColumnsExtent);
		}

		template<detail::range_2d_with_type<Value> Range2D>
		explicit matrix(Range2D&& rng_2d) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (cols != ColumnsExtent)
			{
				throw std::invalid_argument("2D initializer's columns does not match the provided column extent!");
			}

			if (detail::dimension_not_zero_and_non_zero(rows, ColumnsExtent))
			{
				throw std::invalid_argument("Cannot have one side being zero and other side being non-zero!");
			}

			base::init_buf_2d_dynamic(std::forward<Range2D>(rng_2d), rows, ColumnsExtent);
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(
			const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr) // @TODO: ISSUE #20
		{
			if (ColumnsExtent != expr.columns())
			{
				throw std::runtime_error("Columns of expression object doesn't match provided columns extent!");
			}

			base::_rows = expr.rows();
			base::_cols = expr.columns();
			base::_buf.reserve(expr.rows() * expr.columns());

			detail::validate_matrices_same_size(*this, expr);

			for (auto row = std::size_t{ 0 }; row < base::_rows; ++row)
			{
				for (auto col = std::size_t{ 0 }; col < base::_cols; ++col)
				{
					base::_buf.push_back(expr(row, col));
				}
			}
		}

		matrix(std::size_t rows, Value value) // @TODO: ISSUE #20
		{
			base::_rows = rows;
			base::_cols = ColumnsExtent;

			detail::allocate_1d_buf_if_vector(base::_buf, rows, ColumnsExtent, value);
		}

		matrix(std::size_t rows, identity_matrix_tag) // @TODO: ISSUE #20
		{
			base::init_identity(rows, ColumnsExtent);
		}
	};
} // namespace matrixpp