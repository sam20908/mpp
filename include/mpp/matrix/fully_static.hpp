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

		template<bool CheckRangeSize, typename RangeValue>
		void assign_helper(auto&& range_2d) // @TODO: ISSUE #20
		{
			// This checks if the dimensions of the range is equal to the extents

			auto begin      = base::_buffer.begin();
			const auto rows = std::ranges::size(range_2d);

			constexpr auto is_moved_from             = std::is_rvalue_reference_v<decltype(range_2d)>;
			constexpr auto range_has_same_value_type = std::is_same_v<std::remove_cvref_t<Value>, RangeValue>;

			if constexpr (CheckRangeSize)
			{
				if (rows != RowsExtent)
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			if constexpr (is_moved_from)
			{
				for (auto&& row : range_2d)
				{
					const auto current_columns = std::ranges::size(std::move(row));

					if constexpr (CheckRangeSize)
					{
						if (current_columns != ColumnsExtent)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (range_has_same_value_type)
					{
						std::ranges::move(std::move(row), begin);
						begin += ColumnsExtent;
					}
					else
					{
						std::ranges::transform(std::move(row), begin, [](auto&& row_value) {
							return static_cast<Value>(std::move(row_value));
						});

						begin += ColumnsExtent;
					}
				}
			}
			else
			{
				for (const auto& row : range_2d)
				{
					const auto current_columns = std::ranges::size(row);

					if constexpr (CheckRangeSize)
					{
						if (current_columns != ColumnsExtent)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (range_has_same_value_type)
					{
						std::ranges::copy(row, begin);
						begin += ColumnsExtent;
					}
					else
					{
						std::ranges::transform(row, begin, [](const auto& row_value) {
							return static_cast<Value>(row_value);
						});

						begin += ColumnsExtent;
					}
				}
			}

			base::_rows    = RowsExtent;
			base::_columns = ColumnsExtent;
		}

	public:
		using base::operator=;

		matrix()
		{
			fill_buffer_with_value(Value{});
		}

		template<std::convertible_to<Value> InitializerListValue>
		explicit matrix(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			assign_helper<true, InitializerListValue>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		explicit matrix(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			assign_helper<true, detail::range_2d_value_t<Range2D>>(std::forward<Range2D>(range_2d));
		}

		explicit matrix(const std::array<std::array<Value, ColumnsExtent>, RowsExtent>& array_2d) // @TODO: ISSUE #20
		{
			assign_helper<false, Value>(array_2d);
		}

		explicit matrix(std::array<std::array<Value, ColumnsExtent>, RowsExtent>&& array_2d) // @TODO: ISSUE #20
		{
			assign_helper<false, Value>(std::move(array_2d));
		}

		template<typename Expr, std::size_t ExprRowsExtent, std::size_t ExprColumnsExtent>
		explicit matrix(
			const detail::expr_base<Expr, Value, ExprRowsExtent, ExprColumnsExtent>& expr) // @TODO: ISSUE #20
		{
			base::_rows    = RowsExtent;
			base::_columns = ColumnsExtent;

			detail::validate_matrices_same_size(*this, expr);

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
			base::_rows    = RowsExtent;
			base::_columns = ColumnsExtent;

			fill_buffer_with_value(value);
		}

		explicit matrix(identity_matrix_tag,
			const Value& zero_value = Value{ 0 },
			const Value& one_value  = Value{ 1 }) // @TODO: ISSUE #20
		{
			detail::validate_dimensions_for_identity_matrix(RowsExtent, ColumnsExtent);

			base::_rows    = RowsExtent;
			base::_columns = ColumnsExtent;
			fill_buffer_with_value(zero_value);

			detail::make_identity_buffer(base::_buffer, RowsExtent, one_value);
		}

		template<detail::invocable_with_return_type<Value> Callable>
		explicit matrix(Callable&& callable) // @TODO: ISSUE #20
		{
			base::_rows    = RowsExtent;
			base::_columns = ColumnsExtent;

			std::ranges::generate(base::_buffer, std::forward<Callable>(callable));
		}

		template<std::convertible_to<Value> InitializerListValue>
		void assign(
			std::initializer_list<std::initializer_list<InitializerListValue>> initializer_list_2d) // @TODO: ISSUE #20
		{
			assign_helper<true, InitializerListValue>(initializer_list_2d);
		}

		template<detail::range_2d_with_value_type_convertible_to<Value> Range2D>
		void assign(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			assign_helper<true, detail::range_2d_value_t<Range2D>>(std::forward<Range2D>(range_2d));
		}

		void assign(const std::array<std::array<Value, ColumnsExtent>, RowsExtent>& array_2d) // @TODO: ISSUE #20
		{
			assign_helper<false, Value>(array_2d);
		}

		void assign(std::array<std::array<Value, ColumnsExtent>, RowsExtent>&& array_2d) // @TODO: ISSUE #20
		{
			assign_helper<false, Value>(std::move(array_2d));
		}
	};
} // namespace mpp
