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
#include <mpp/detail/utility/utility.hpp>

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Value, std::size_t ColumnsExtent, typename Allocator>
	class matrix<Value, dynamic, ColumnsExtent, Allocator> :
		public detail::matrix_dynamic_base<matrix<Value, dynamic, ColumnsExtent, Allocator>,
			typename configuration<override>::dynamic_rows_buffer<Value, 1, ColumnsExtent>,
			Value,
			dynamic,
			ColumnsExtent,
			Allocator>
	{
		using base = detail::matrix_dynamic_base<matrix<Value, dynamic, ColumnsExtent, Allocator>,
			typename configuration<override>::dynamic_rows_buffer<Value, 1, ColumnsExtent>,
			Value,
			dynamic,
			ColumnsExtent,
			Allocator>;

	public:
		using base::operator=;

		matrix() noexcept(noexcept(Allocator())) : base(0, ColumnsExtent, Allocator{}) {} // @TODO: ISSUE #20

		explicit matrix(const Allocator& allocator) noexcept : base(0, ColumnsExtent, allocator) {} // @TODO: ISSUE #20

		matrix(const matrix& right, const Allocator& allocator) :
			base(right.rows_, right.columns_, right.buffer_, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(matrix&& right, const Allocator& allocator) :
			base(std::move(right.rows_),
				std::move(right.columns_),
				std::move(right.buffer_),
				allocator) // @TODO: ISSUE #20
		{
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		explicit matrix(Matrix&& matrix, const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			const auto mat_rows    = std::forward<Matrix>(matrix).rows();
			const auto mat_columns = std::forward<Matrix>(matrix).columns();

			base::template assign_and_insert_from_1d_range<false, detail::configuration_use_safe, false>(mat_rows,
				mat_columns,
				std::forward<Matrix>(matrix));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		explicit matrix(Matrix&& matrix, unsafe_tag, const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			const auto mat_rows    = std::forward<Matrix>(matrix).rows();
			const auto mat_columns = std::forward<Matrix>(matrix).columns();

			base::template assign_and_insert_from_1d_range<false, false, false>(mat_rows,
				mat_columns,
				std::forward<Matrix>(matrix));
		}

		template<detail::range_1d_with_value_type_convertible_to<Value> Range>
		explicit matrix(std::size_t rows,
			std::size_t columns,
			Range&& range,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<false,
				detail::configuration_use_safe,
				detail::configuration_use_safe>(rows, columns, std::forward<Range>(range));
		}

		template<detail::range_1d_with_value_type_convertible_to<Value> Range>
		explicit matrix(std::size_t rows,
			std::size_t columns,
			Range&& range,
			unsafe_tag,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<false, false, false>(rows,
				columns,
				std::forward<Range>(range));
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
			base::template assign_and_insert_from_2d_range<false, detail::configuration_use_safe, false>(
				initializer_list_2d);
		}

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d,
			unsafe_tag,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d, const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, detail::configuration_use_safe, false>(
				std::forward<Range2D>(range_2d));
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d, unsafe_tag, const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Range2D>(range_2d));
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


		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			unsafe_tag,
			const Allocator& allocator = Allocator{}) :
			base(0, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			base::initialize_from_expression_unchecked(expr.rows(), ColumnsExtent, expr);
		}

		matrix(std::size_t rows, const Value& value, const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, rows * ColumnsExtent, value, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows,
			identity_tag,
			const Value& zero_value    = Value{ 0 },
			const Value& one_value     = Value{ 1 },
			const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			detail::make_identity_buffer<detail::configuration_use_safe>(base::buffer_,
				rows,
				ColumnsExtent,
				zero_value,
				one_value);
		}

		matrix(std::size_t rows,
			identity_tag,
			unsafe_tag,
			const Value& zero_value    = Value{ 0 },
			const Value& one_value     = Value{ 1 },
			const Allocator& allocator = Allocator{}) :
			base(rows, ColumnsExtent, allocator) // @TODO: ISSUE #20
		{
			detail::make_identity_buffer<false>(base::buffer_, rows, ColumnsExtent, zero_value, one_value);
		}

		// @FIXME: Allow callable's value return be convertible to value type
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
			base::template assign_and_insert_from_2d_range<false, detail::configuration_use_safe, false>(
				initializer_list_2d);
		}

		template<std::convertible_to<Value> InitializerListValue>
		void assign(std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d,
			unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, detail::configuration_use_safe, false>(
				std::forward<Range2D>(range_2d));
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		void assign(Matrix&& matrix)
		{
			const auto mat_rows    = std::forward<Matrix>(matrix).rows();
			const auto mat_columns = std::forward<Matrix>(matrix).columns();

			base::template assign_and_insert_from_1d_range<false, detail::configuration_use_safe, false>(mat_rows,
				mat_columns,
				std::forward<Matrix>(matrix));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		void assign(Matrix&& matrix, unsafe_tag)
		{
			const auto mat_rows    = std::forward<Matrix>(matrix).rows();
			const auto mat_columns = std::forward<Matrix>(matrix).columns();

			base::template assign_and_insert_from_1d_range<false, false, false>(mat_rows,
				mat_columns,
				std::forward<Matrix>(matrix));
		}
	};
} // namespace mpp
