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
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/exception_messages.hpp>
#include <mpp/detail/utility/validators.hpp>

#include <concepts>
#include <initializer_list>
#include <ranges>
#include <type_traits>
#include <utility>

namespace mpp
{
	template<detail::arithmetic Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
	class matrix :
		public detail::matrix_base<matrix<Value, RowsExtent, ColumnsExtent, Allocator>,
			std::array<Value, RowsExtent * ColumnsExtent>,
			Value,
			RowsExtent,
			ColumnsExtent,
			Allocator>
	{
		using base = detail::matrix_base<matrix<Value, RowsExtent, ColumnsExtent, Allocator>,
			std::array<Value, RowsExtent * ColumnsExtent>,
			Value,
			RowsExtent,
			ColumnsExtent,
			Allocator>;

		void fill_buffer_with_value(const Value& value) // @TODO: ISSUE #20
		{
			std::ranges::fill(base::_buffer, value);
		}

	public:
		using base::operator=;

		matrix()
		{
			fill_buffer_with_value(Value{});
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		explicit matrix(Matrix&& matrix) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<true, true, false>(std::forward<Matrix>(matrix).rows(),
				std::forward<Matrix>(matrix).columns(),
				std::forward<Matrix>(matrix));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		explicit matrix(Matrix&& matrix, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<false, false, false>(std::forward<Matrix>(matrix).rows(),
				std::forward<Matrix>(matrix).columns(),
				std::forward<Matrix>(matrix));
		}

		template<detail::range_1d_with_value_type_convertible_to<Value> Range>
		explicit matrix(std::size_t rows, std::size_t columns, Range&& range) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<true, true, true>(rows, columns, std::forward<Range>(range));
		}

		template<detail::range_1d_with_value_type_convertible_to<Value> Range>
		explicit matrix(std::size_t rows, std::size_t columns, Range&& range, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<false, false, false>(rows,
				columns,
				std::forward<Range>(range));
		}

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<true, true, false>(initializer_list_2d);
		}

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d,
			unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<true, true, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::array_2d_with_value_convertible_to<RowsExtent, ColumnsExtent, Value> Array2D>
		explicit matrix(Array2D&& array_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Array2D>(array_2d));
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(
			const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr) // @TODO: ISSUE #20
		{
			detail::validate_matrices_same_size(*this, expr);

			for (auto row = std::size_t{}, index = std::size_t{}; row < RowsExtent; ++row)
			{
				for (auto column = std::size_t{}; column < ColumnsExtent; ++column)
				{
					base::_buffer[index++] = expr(row, column);
				}
			}
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			unsafe_tag) // @TODO: ISSUE #20
		{
			for (auto row = std::size_t{}, index = std::size_t{}; row < RowsExtent; ++row)
			{
				for (auto column = std::size_t{}; column < ColumnsExtent; ++column)
				{
					base::_buffer[index++] = expr(row, column);
				}
			}
		}

		explicit matrix(const Value& value) // @TODO: ISSUE #20
		{
			fill_buffer_with_value(value);
		}

		explicit matrix(identity_tag,
			const Value& zero_value = Value{ 0 },
			const Value& one_value  = Value{ 1 }) // @TODO: ISSUE #20
		{
			detail::template make_identity_buffer<true>(base::_buffer,
				RowsExtent,
				ColumnsExtent,
				zero_value,
				one_value);
		}

		explicit matrix(identity_tag,
			unsafe_tag,
			const Value& zero_value = Value{ 0 },
			const Value& one_value  = Value{ 1 }) // @TODO: ISSUE #20
		{
			fill_buffer_with_value(zero_value);

			detail::template make_identity_buffer<false>(base::_buffer,
				RowsExtent,
				ColumnsExtent,
				zero_value,
				one_value);
		}

		template<detail::invocable_with_return_type<Value> Callable>
		explicit matrix(Callable&& callable) // @TODO: ISSUE #20
		{
			std::ranges::generate(base::_buffer, std::forward<Callable>(callable));
		}

		template<std::convertible_to<Value> InitializerListValue>
		void assign(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<true, true, false>(initializer_list_2d);
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
			base::template assign_and_insert_from_2d_range<true, true, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Range2D>(range_2d));
		}

		template<detail::array_2d_with_value_convertible_to<RowsExtent, ColumnsExtent, Value> Array2D>
		void assign(Array2D&& array_2d) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_2d_range<false, false, false>(std::forward<Array2D>(array_2d));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		void assign(Matrix&& matrix) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<true, true, false>(std::forward<Matrix>(matrix).rows(),
				std::forward<Matrix>(matrix).columns(),
				std::forward<Matrix>(matrix));
		}

		template<detail::matrix_with_value_convertible_to<Value> Matrix>
		void assign(Matrix&& matrix, unsafe_tag) // @TODO: ISSUE #20
		{
			base::template assign_and_insert_from_1d_range<false, false, false>(std::forward<Matrix>(matrix).rows(),
				std::forward<Matrix>(matrix).columns(),
				std::forward<Matrix>(matrix));
		}
	};
} // namespace mpp
