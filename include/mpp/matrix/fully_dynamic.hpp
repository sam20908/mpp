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
	template<detail::arithmetic Value, typename Allocator>
	class matrix<Value, std::dynamic_extent, std::dynamic_extent, Allocator> :
		public detail::matrix_dynamic_base<matrix<Value, std::dynamic_extent, std::dynamic_extent, Allocator>,
			std::vector<Value, Allocator>,
			Value,
			std::dynamic_extent,
			std::dynamic_extent,
			Allocator>
	{
		using base = detail::matrix_dynamic_base<matrix<Value, std::dynamic_extent, std::dynamic_extent, Allocator>,
			std::vector<Value, Allocator>,
			Value,
			std::dynamic_extent,
			std::dynamic_extent,
			Allocator>;

		template<typename RangeValue>
		void assign_helper(auto&& range_2d) // @TODO: ISSUE #20
		{
			// This checks for unequal columns

			const auto back_inserter   = std::back_inserter(base::_buffer);
			const auto begin           = std::ranges::begin(range_2d);
			const auto rows            = std::ranges::size(range_2d);
			const auto initial_columns = rows > 0 ? std::ranges::size(*begin) : std::size_t{};

			base::_buffer.reserve(rows * initial_columns);

			constexpr auto is_moved_from             = std::is_rvalue_reference_v<decltype(range_2d)>;
			constexpr auto range_has_same_value_type = std::is_same_v<std::remove_cvref_t<Value>, RangeValue>;

			if constexpr (is_moved_from)
			{
				for (auto&& row : range_2d)
				{
					const auto current_columns = std::ranges::size(std::move(row));

					if (current_columns != initial_columns)
					{
						throw std::invalid_argument(detail::INITIALIZER_UNEQUAL_ROWS);
					}

					if constexpr (range_has_same_value_type)
					{
						std::ranges::move(std::move(row), back_inserter);
					}
					else
					{
						std::ranges::transform(std::move(row), back_inserter, [](auto&& row_value) {
							return static_cast<Value>(std::move(row_value));
						});
					}
				}
			}
			else
			{
				for (const auto& row : range_2d)
				{
					const auto current_columns = std::ranges::size(row);

					if (current_columns != initial_columns)
					{
						throw std::invalid_argument(detail::INITIALIZER_UNEQUAL_ROWS);
					}

					if constexpr (range_has_same_value_type)
					{
						std::ranges::copy(row, back_inserter);
					}
					else
					{
						std::ranges::transform(row, back_inserter, [](const auto& row_value) {
							return static_cast<Value>(row_value);
						});
					}
				}
			}

			base::_rows    = rows;
			base::_columns = initial_columns;
		}

	public:
		using base::operator=;

		matrix() : base(0, 0, Allocator{}) {} // @TODO: ISSUE #20

		explicit matrix(const Allocator& allocator) : base(0, 0, allocator) {} // @TODO: ISSUE #20

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

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d,
			const Allocator allocator = Allocator{}) :
			base(0, 0, allocator) // @TODO: ISSUE #20
		{
			assign_helper<InitializerListValue>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d, const Allocator allocator = Allocator{}) :
			base(0, 0, allocator) // @TODO: ISSUE #20
		{
			assign_helper<detail::range_2d_value_t<Range2D>>(std::forward<Range2D>(range_2d));
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr,
			const Allocator allocator = Allocator{}) :
			base(0, 0, allocator) // @TODO: ISSUE #20
		{
			base::initialize_from_expression_unchecked(expr.rows(), expr.columns(), expr);
		}

		matrix(std::size_t rows, std::size_t columns, const Allocator& allocator = Allocator{}) :
			base(rows, columns, rows * columns, Value{}, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows, std::size_t columns, const Value& value, const Allocator& allocator = Allocator{}) :
			base(rows, columns, rows * columns, value, allocator) // @TODO: ISSUE #20
		{
		}

		matrix(std::size_t rows,
			std::size_t columns,
			identity_matrix_tag,
			const Value& zero_value    = Value{ 0 },
			const Value& one_value     = Value{ 1 },
			const Allocator& allocator = Allocator{}) :
			base(rows, columns, identity_matrix_tag{}, zero_value, one_value, allocator) // @TODO: ISSUE #20
		{
		}

		template<detail::invocable_with_return_type<Value> Callable>
		matrix(std::size_t rows, std::size_t columns, Callable&& callable, const Allocator& allocator = Allocator{}) :
			base(rows, columns, allocator) // @TODO: ISSUE #20
		{
			base::initialize_buffer_from_callable_unchecked(rows, columns, std::forward<Callable>(callable));
		}

		template<std::convertible_to<Value> InitializerListValue>
		void assign(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			assign_helper<InitializerListValue>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			assign_helper<detail::range_2d_value_t<Range2D>>(std::forward<Range2D>(range_2d));
		}
	};
} // namespace mpp