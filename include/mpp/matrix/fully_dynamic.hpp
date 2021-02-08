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

#include <mpp/detail/constraints.hpp>
#include <mpp/detail/matrix_base.hpp>
#include <mpp/detail/matrix_def.hpp>

#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace mpp
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
			const auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			base::init_buf_2d_dynamic(init_2d, rows, cols);
		}

		template<detail::range_2d_with_type<Value> Range2D>
		explicit matrix(Range2D&& rng_2d) // @TODO: ISSUE #20
		{
			const auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			base::init_buf_2d_dynamic(std::forward<Range2D>(rng_2d), rows, cols);
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(
			const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr) // @TODO: ISSUE #20
		{
			base::init_expr_dynamic(expr.rows(), expr.columns(), expr, false);
		}

		matrix(std::size_t rows, std::size_t columns) // @TODO: ISSUE #20
		{
			base::_rows = rows;
			base::_cols = columns;

			// @TODO: ISSUE #129
			detail::allocate_1d_buf_if_vector(base::_buf, rows, columns, Value{ 0 });
		}

		matrix(std::size_t rows, std::size_t columns, Value value) // @TODO: ISSUE #20
		{
			base::_rows = rows;
			base::_cols = columns;

			detail::allocate_1d_buf_if_vector(base::_buf, rows, columns, value);
		}

		matrix(std::size_t rows, std::size_t columns, identity_matrix_tag) // @TODO: ISSUE #20
		{
			base::init_identity(rows, columns);
		}
	};
} // namespace mpp