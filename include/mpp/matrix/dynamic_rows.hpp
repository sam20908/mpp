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

#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Value, std::size_t ColumnsExtent, typename Allocator>
	class matrix<Value, std::dynamic_extent, ColumnsExtent, Allocator> :
		public detail::
			matrix_dynamic_base<std::vector<Value, Allocator>, Value, std::dynamic_extent, ColumnsExtent, Allocator>
	{
		using base = detail::
			matrix_dynamic_base<std::vector<Value, Allocator>, Value, std::dynamic_extent, ColumnsExtent, Allocator>;

	public:
		using base::operator=;

		matrix() : base(0, ColumnsExtent, Allocator{}) {} // @TODO: ISSUE #20

		explicit matrix(const Allocator& allocator) : base(0, ColumnsExtent, allocator) {} // @TODO: ISSUE #20

		matrix(const matrix& right, const Allocator& allocator) : base(right, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(matrix&& right, const Allocator& allocator) : base(std::move(right), allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, Value{}, allocator) // @TODO: ISSUE #20
		{
		}

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d,
			const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (cols != ColumnsExtent)
			{
				throw std::invalid_argument("Initializer's columns does not match the provided column extent!");
			}

			base::init_buf_2d_dynamic_without_check(init_2d, rows, ColumnsExtent);
		}

		template<detail::range_2d_with_type<Value> Range2D>
		explicit matrix(Range2D&& rng_2d, const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (cols != ColumnsExtent)
			{
				throw std::invalid_argument("Initializer's columns does not match the provided column extent!");
			}

			base::init_buf_2d_dynamic_without_check(std::forward<Range2D>(rng_2d), rows, ColumnsExtent);
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			if (ColumnsExtent != expr.columns())
			{
				throw std::invalid_argument("Columns of expression object doesn't match provided columns extent!");
			}

			base::init_expr_dynamic_without_check(expr.rows(), ColumnsExtent, expr);
		}

		matrix(std::size_t rows, const Value& value, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, value, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows, identity_matrix_tag, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, identity_matrix_tag{}, allocator) // @TODO: ISSUE #20
		{
		}

		template<detail::invocable_with_return_type<Value> Callable>
		matrix(std::size_t rows, Callable&& callable, const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			base::init_buf_from_callable_dynamic(rows, ColumnsExtent, std::forward<Callable>(callable));
		}
	};
} // namespace mpp