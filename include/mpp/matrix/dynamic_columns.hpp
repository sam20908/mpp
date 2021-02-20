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
	template<detail::arithmetic Value, std::size_t RowsExtent, typename Allocator>
	class matrix<Value, RowsExtent, std::dynamic_extent, Allocator> :
		public detail::matrix_base<std::vector<Value, Allocator>, Value, RowsExtent, std::dynamic_extent, Allocator>
	{
		using base =
			detail::matrix_base<std::vector<Value, Allocator>, Value, RowsExtent, std::dynamic_extent, Allocator>;

	public:
		using allocator_type = Allocator;

		matrix() : base(RowsExtent, 0, Allocator{}) {} // @TODO: ISSUE #20

		explicit matrix(const Allocator& allocator) : base(RowsExtent, 0, allocator) {} // @TODO: ISSUE #20

		matrix(std::size_t columns, const Allocator& allocator = Allocator{}) :
			base(RowsExtent, columns, Value{}, allocator) // @TODO: ISSUE #20
		{
		}

		explicit matrix(std::initializer_list<std::initializer_list<Value>> init_2d,
			const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(init_2d);

			if (rows != RowsExtent)
			{
				throw std::invalid_argument("Initializer's rows does not match the provided row extent!");
			}

			base::init_buf_2d_dynamic_without_check(init_2d, RowsExtent, cols);
		}

		template<detail::range_2d_with_type<Value> Range2D>
		explicit matrix(Range2D&& rng_2d, const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			auto [rows, cols] = detail::range_2d_dimensions(rng_2d);

			if (rows != RowsExtent)
			{
				throw std::invalid_argument("Initializer's rows does not match the provided row extent!");
			}

			base::init_buf_2d_dynamic_without_check(std::forward<Range2D>(rng_2d), RowsExtent, cols);
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			if (RowsExtent != expr.rows())
			{
				throw std::invalid_argument("Rows of expression object doesn't match provided rows extent!");
			}

			base::init_expr_dynamic_without_check(RowsExtent, expr.columns(), expr);
		}

		matrix(std::size_t columns, const Value& value, const Allocator& allocator = Allocator{}) :
			base(RowsExtent, columns, value, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t columns, identity_matrix_tag, const Allocator& allocator = Allocator{}) :
			base(RowsExtent, columns, identity_matrix_tag{}, allocator) // @TODO: ISSUE #20
		{
		}

		template<detail::invocable_with_return_type<Value> Callable>
		matrix(std::size_t columns, Callable&& callable, const Allocator& allocator = Allocator{}) :
			base(allocator) // @TODO: ISSUE #20
		{
			base::init_buf_from_callable_dynamic(RowsExtent, columns, std::forward<Callable>(callable));
		}

		matrix(const matrix& right, const Allocator& allocator) : base(right, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(matrix&& right, const Allocator& allocator) : base(std::move(right), allocator) // @TODO: ISSUE #20
		{
		}

		[[nodiscard]] auto get_allocator() const -> allocator_type // @TODO: ISSUE #20
		{
			return base::_buf.get_allocator();
		}
	};
} // namespace mpp