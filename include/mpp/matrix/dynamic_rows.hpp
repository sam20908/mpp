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

#include <mpp/detail/matrix/matrix_base.hpp>
#include <mpp/detail/matrix/matrix_def.hpp>
#include <mpp/detail/types/constraints.hpp>

#include <initializer_list>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Value, std::size_t ColumnsExtent, typename Allocator>
	class matrix<Value, std::dynamic_extent, ColumnsExtent, Allocator> :
		public detail::matrix_dynamic_base<matrix<Value, std::dynamic_extent, ColumnsExtent, Allocator>,
			std::vector<Value, Allocator>,
			Value,
			std::dynamic_extent,
			ColumnsExtent,
			Allocator>
	{
		using base = detail::matrix_dynamic_base<matrix<Value, std::dynamic_extent, ColumnsExtent, Allocator>,
			std::vector<Value, Allocator>,
			Value,
			std::dynamic_extent,
			ColumnsExtent,
			Allocator>;

	public:
		using base::operator=;

		matrix() : base(0, ColumnsExtent, Allocator{}) {} // @TODO: ISSUE #20

		explicit matrix(const Allocator& allocator) : base(0, ColumnsExtent, allocator) {} // @TODO: ISSUE #20

		matrix(const matrix& right, const Allocator& allocator) :
			base(right._rows, right._columns, right, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(matrix&& right, const Allocator& allocator) :
			base(std::move(right)._rows,
				std::move(right)._columns,
				std::move(right),
				allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, rows * ColumnsExtent, Value{}, allocator) // @TODO: ISSUE #20
		{
		}

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_if_bigger<false, true, false, false>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d, const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_if_bigger<false, true, false, false>(std::forward<Range2D>(range_2d));
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			if (ColumnsExtent != expr.columns())
			{
				throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
			}

			base::initialize_from_expression_unchecked(expr.rows(), ColumnsExtent, expr);
		}

		matrix(std::size_t rows, const Value& value, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, rows * ColumnsExtent, value, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows,
			identity_matrix_tag,
			const Value& zero_value    = Value{ 0 },
			const Value& one_value     = Value{ 1 },
			const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, identity_matrix_tag{}, zero_value, one_value, allocator) // @TODO: ISSUE #20
		{
		}

		template<detail::invocable_with_return_type<Value> Callable>
		matrix(std::size_t rows, Callable&& callable, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::initialize_buffer_from_callable_unchecked(rows, ColumnsExtent, std::forward<Callable>(callable));
		}

		template<std::convertible_to<Value> InitializerListValue>
		void assign(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_if_bigger<false, true, false, false>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_if_bigger<false, true, false, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::matrix_like_with_value_type_convertible_to<Value> Matrix>
		void assign(Matrix&& matrix)
		{
			base::template assign_and_insert_from_matrix<false, true>(std::forward<Matrix>(matrix));
		}
	};
} // namespace mpp