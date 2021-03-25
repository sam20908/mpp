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

#include <mpp/detail/expr/expr_base.hpp>
#include <mpp/detail/matrix/matrix_iterator.hpp>
#include <mpp/detail/types/constraints.hpp>
#include <mpp/detail/types/type_traits.hpp>
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/detail/utility/validators.hpp>
#include <mpp/utility/configuration.hpp>
#include <mpp/utility/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

namespace mpp::detail
{
	/**
	 * Base matrix class to store internal data and define common member functions
	 */
	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	class matrix_base : public expr_base<Derived, Value, RowsExtent, ColumnsExtent>, public traits<Value>
	{
		using base = expr_base<Derived, Value, RowsExtent, ColumnsExtent>;

	protected:
		Buffer _buffer; // Don't default initailize because value type might not be DefaultConstructible
		std::size_t _rows{ RowsExtent == dynamic ? std::size_t{} : RowsExtent };
		std::size_t _columns{ ColumnsExtent == dynamic ? std::size_t{} : ColumnsExtent };

		// This is needed for fully static matrices
		matrix_base() = default;

		template<typename... Args>
		matrix_base(std::size_t rows, std::size_t columns, Args&&... args) :
			_buffer(std::forward<Args>(args)...),
			_rows{ rows },
			_columns{ columns } // @TODO: ISSUE #20
		{
		}

		template<bool CheckAgainstCurrentRows, bool CheckAgainstCurrentColumns, bool CheckUnequalColumns, bool Unsafe>
		void assign_and_insert_from_2d_range(auto&& range_2d)
		{
			using range_2d_t = decltype(range_2d);

			const auto range_rows = std::ranges::size(std::forward<range_2d_t>(range_2d));

			if constexpr (!Unsafe && CheckAgainstCurrentRows)
			{
				if (range_rows != _rows)
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			auto range_begin            = std::ranges::begin(range_2d);
			const auto range_columns    = range_rows > 0 ? std::ranges::size(*range_begin) : std::size_t{};
			const auto range_total_size = range_rows * range_columns;
			const auto buffer_size      = _buffer.size();

			constexpr auto range_is_moved   = std::is_rvalue_reference_v<range_2d_t>;
			constexpr auto buffer_is_vector = is_vector<Buffer>::value;
			constexpr auto range_has_same_value_type =
				std::is_same_v<range_2d_value_t<std::remove_cvref_t<range_2d_t>>, Value>;

			if constexpr (buffer_is_vector)
			{
				_buffer.reserve(range_total_size);
			}

			// Assign all elements (until the need to insert for dynamic matrices)
			const auto min_rows = (std::min)(range_rows, _rows);
			auto buffer_begin   = _buffer.begin();

			for (auto row = std::size_t{}; row < min_rows; ++row)
			{
				const auto current_columns = std::ranges::size(*range_begin);

				if constexpr (!Unsafe && CheckAgainstCurrentColumns)
				{
					if (current_columns != _columns)
					{
						throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
					}
				}

				if constexpr (!Unsafe && CheckUnequalColumns)
				{
					if (current_columns != range_columns)
					{
						throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
					}
				}

				const auto rooms_available_from_current = buffer_size - row * range_columns;

				if (rooms_available_from_current >= range_columns)
				{
					if constexpr (range_has_same_value_type)
					{
						if constexpr (range_is_moved)
						{
							std::ranges::move(*range_begin, buffer_begin);
						}
						else
						{
							std::ranges::copy(*range_begin, buffer_begin);
						}
					}
					else
					{
						// @TODO: Check if this *really* is perfect forwarding values
						std::ranges::transform(*range_begin, buffer_begin, [](auto&& value) -> decltype(auto) {
							return static_cast<Value>(std::forward<decltype(value)>(value));
						});
					}
				}
				else
				{
					// Only dynamic matrices reach here
					// We've gotten to the point where some or rest of the elements needs to be inserted

					if constexpr (buffer_is_vector)
					{
						const auto row_assign_begin = std::ranges::begin(*range_begin);
						const auto row_assign_end =
							row_assign_begin + static_cast<difference_type>(rooms_available_from_current);

						// Assign the elements to rest of available space
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_begin, row_assign_end, buffer_begin);
							}
							else
							{
								std::copy(row_assign_begin, row_assign_end, buffer_begin);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_begin,
								row_assign_end,
								buffer_begin,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Value>(std::forward<decltype(value)>(value));
								});
						}

						const auto elements_to_insert = range_columns - rooms_available_from_current;
						const auto row_end = row_assign_end + static_cast<difference_type>(elements_to_insert);
						const auto buffer_back_inserter = std::back_inserter(_buffer);

						// Insert rest of the elements in the current row
						if constexpr (range_has_same_value_type)
						{
							if constexpr (range_is_moved)
							{
								std::move(row_assign_end, row_end, buffer_back_inserter);
							}
							else
							{
								std::copy(row_assign_end, row_end, buffer_back_inserter);
							}
						}
						else
						{
							// @TODO: Check if this *really* is perfect forwarding values
							std::transform(row_assign_end,
								row_end,
								buffer_back_inserter,
								[](auto&& value) -> decltype(auto) {
									return static_cast<Value>(std::forward<decltype(value)>(value));
								});
						}

						_rows = ++row; // Use the inserter loop to insert rest of the elements
						++range_begin;

						break;
					}
				}

				++range_begin;
				buffer_begin += static_cast<difference_type>(range_columns);
			}

			// Insert every element after (only dynamic matrices reach this point)
			if constexpr (buffer_is_vector)
			{
				const auto buffer_back_inserter = std::back_inserter(_buffer);

				for (auto row = _rows; row < range_rows; ++row)
				{
					const auto current_columns = std::ranges::size(*range_begin);

					if constexpr (!Unsafe && CheckAgainstCurrentColumns)
					{
						if (current_columns != _columns)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (!Unsafe && CheckUnequalColumns)
					{
						if (current_columns != range_columns)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (range_is_moved)
					{
						std::ranges::move(*range_begin, buffer_back_inserter);
					}
					else
					{
						std::ranges::copy(*range_begin, buffer_back_inserter);
					}

					++range_begin;
				}
			}

			if constexpr (buffer_is_vector)
			{
				if (range_total_size < buffer_size)
				{
					_buffer.resize(range_total_size);
				}
			}

			_rows    = range_rows;
			_columns = range_columns;
		}

		template<bool CheckAgainstCurrentRows, bool CheckAgainstCurrentColumns, bool Unsafe, typename Range>
		void assign_and_insert_from_1d_range(std::size_t rows, std::size_t columns, Range&& range)
		{
			if constexpr (!Unsafe && CheckAgainstCurrentRows)
			{
				if (rows != _rows)
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			if constexpr (!Unsafe && CheckAgainstCurrentColumns)
			{
				if (columns != _columns)
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			const auto buffer_size = _buffer.size();
			const auto range_size  = rows * columns;

			if constexpr (!Unsafe)
			{
				// Last resort safety check
				if (range_size != std::ranges::size(range))
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			constexpr auto range_is_moved            = std::is_rvalue_reference_v<Range>;
			constexpr auto buffer_is_vector          = is_vector<Buffer>::value;
			constexpr auto range_has_same_value_type = std::is_same_v<std::ranges::range_value_t<Range>, Value>;

			if constexpr (buffer_is_vector)
			{
				_buffer.reserve(range_size);
			}

			const auto max_elements_to_assign = (std::min)(range_size, buffer_size);
			const auto assign_begin           = std::ranges::begin(range);
			const auto assign_end             = assign_begin + static_cast<difference_type>(max_elements_to_assign);
			const auto buffer_begin           = _buffer.begin();

			// Try to assign the all elements it can
			if constexpr (range_has_same_value_type)
			{
				if constexpr (range_is_moved)
				{
					std::move(assign_begin, assign_end, buffer_begin);
				}
				else
				{
					std::copy(assign_begin, assign_end, buffer_begin);
				}
			}
			else
			{
				// @TODO: Check if this *really* is perfect forwarding values
				std::transform(assign_begin, assign_end, _buffer.begin(), [](auto&& value) -> decltype(auto) {
					return static_cast<Value>(std::forward<decltype(value)>(value));
				});
			}

			const auto range_end            = std::ranges::end(range);
			const auto buffer_back_inserter = std::back_inserter(_buffer);

			// Insert the leftovers (only dynamic matrices needs this)
			if constexpr (buffer_is_vector)
			{
				if constexpr (range_has_same_value_type)
				{
					if constexpr (range_is_moved)
					{
						std::move(assign_end, range_end, buffer_back_inserter);
					}
					else
					{
						std::copy(assign_end, range_end, buffer_back_inserter);
					}
				}
				else
				{
					// @TODO: Check if this *really* is perfect forwarding values
					std::transform(assign_end, range_end, buffer_back_inserter, [](auto&& value) -> decltype(auto) {
						return static_cast<Value>(std::forward<decltype(value)>(value));
					});
				}

				if (range_size < buffer_size)
				{
					_buffer.resize(range_size);
				}
			}

			_rows    = rows;
			_columns = columns;
		}

	public:
		using buffer_type = Buffer;

		using value_type             = Value;
		using reference              = value_type&;
		using const_reference        = const value_type&;
		using pointer                = typename buffer_type::pointer;
		using const_pointer          = typename buffer_type::const_pointer;
		using iterator               = matrix_iterator<typename Buffer::iterator>;
		using const_iterator         = matrix_iterator<typename Buffer::const_iterator>;
		using reverse_iterator       = matrix_iterator<typename Buffer::reverse_iterator>;
		using const_reverse_iterator = matrix_iterator<typename Buffer::const_reverse_iterator>;
		using difference_type        = typename buffer_type::difference_type;
		using size_type              = std::size_t;

		matrix_base(const matrix_base&) = default; // @TODO: ISSUE #20
		matrix_base(matrix_base&&)      = default; // @TODO: ISSUE #20

		matrix_base(const matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>& right,
			const Allocator& allocator) :
			_buffer(right._buffer, allocator), // @TODO: ISSUE #20
			_rows{ right._rows },
			_columns{ right._columns }
		{
		}

		matrix_base(matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>&& right,
			const Allocator& allocator) :
			_buffer(std::move(right._buffer), allocator), // @TODO: ISSUE #20
			_rows{ std::move(right._rows) },
			_columns{ std::move(right._columns) }
		{
		}

		auto operator=(const matrix_base&) -> matrix_base& = default; // @TODO: ISSUE #20
		auto operator=(matrix_base&&) -> matrix_base& = default;      // @TODO: ISSUE #20

		[[nodiscard]] auto data() -> pointer // @TODO: ISSUE #20
		{
			return _buffer.data();
		}

		[[nodiscard]] auto data() const -> const_pointer // @TODO: ISSUE #20
		{
			return _buffer.data();
		}

		[[nodiscard]] auto begin() -> iterator // @TODO: ISSUE #20
		{
			return iterator(_buffer.begin(), _columns);
		}

		[[nodiscard]] auto begin() const -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cbegin(), _columns);
		}

		[[nodiscard]] auto end() -> iterator // @TODO: ISSUE #20
		{
			return iterator(_buffer.end(), _columns);
		}

		[[nodiscard]] auto end() const -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cend(), _columns);
		}

		[[nodiscard]] auto rbegin() -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(_buffer.rbegin(), _columns);
		}

		[[nodiscard]] auto rbegin() const -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crbegin(), _columns);
		}

		[[nodiscard]] auto rend() -> reverse_iterator // @TODO: ISSUE #20
		{
			return reverse_iterator(_buffer.rend(), _columns);
		}

		[[nodiscard]] auto rend() const -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crend(), _columns);
		}

		[[nodiscard]] auto cbegin() -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cbegin(), _columns);
		}

		[[nodiscard]] auto cbegin() const -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cbegin(), _columns);
		}

		[[nodiscard]] auto cend() -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cend(), _columns);
		}

		[[nodiscard]] auto cend() const -> const_iterator // @TODO: ISSUE #20
		{
			return const_iterator(_buffer.cend(), _columns);
		}

		[[nodiscard]] auto crbegin() -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crbegin(), _columns);
		}

		[[nodiscard]] auto crbegin() const -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crbegin(), _columns);
		}

		[[nodiscard]] auto crend() -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crend(), _columns);
		}

		[[nodiscard]] auto crend() const -> const_reverse_iterator // @TODO: ISSUE #20
		{
			return const_reverse_iterator(_buffer.crend(), _columns);
		}

		[[nodiscard]] auto at(std::size_t row_index, std::size_t column_index) const
			-> const_reference // @TODO: ISSUE #20
		{
			if (row_index >= _rows || column_index >= _columns)
			{
				throw std::out_of_range("Access out of bounds!");
			}

			// Don't use internal buffer's .at because we already performed bounds checking here

			return operator()(row_index, column_index);
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) -> reference // @TODO: ISSUE #20
		{
			const auto index = index_2d_to_1d(_columns, row_index, col_index);

			return _buffer[index];
		}

		[[nodiscard]] auto operator()(std::size_t row_index,
			std::size_t col_index) const -> const_reference // @TODO: ISSUE #20
		{
			const auto index = index_2d_to_1d(_columns, row_index, col_index);

			return _buffer[index];
		}

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return _rows;
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return _columns;
		}

		[[nodiscard]] auto front() -> reference // @TODO: ISSUE #20
		{
			return _buffer.front();
		}

		[[nodiscard]] auto front() const -> const_reference // @TODO: ISSUE #20
		{
			return _buffer.front();
		}

		[[nodiscard]] auto back() -> reference // @TODO: ISSUE #20
		{
			return _buffer.back();
		}

		[[nodiscard]] auto back() const -> const_reference // @TODO: ISSUE #20
		{
			return _buffer.back();
		}

		[[nodiscard]] auto size() const -> std::size_t // @TODO: ISSUE #20
		{
			return _buffer.size();
		}

		[[nodiscard]] auto max_size() const -> std::size_t // @TODO: ISSUE #20
		{
			return _buffer.max_size();
		}

		[[nodiscard]] auto empty() const -> bool // @TODO: ISSUE #20
		{
			return _buffer.empty();
		}

		template<typename Range2D>
		auto operator=(Range2D&& range_2d) -> matrix_base& // @TODO: ISSUE #20
		{
			base::expr_mutable_obj().assign(std::forward<Range2D>(range_2d));
			return *this;
		}

		void swap(matrix_base& right) // @TODO: ISSUE #20
		{
			// Don't swap with the same object
			if (&right != this)
			{
				using std::swap;

				swap(_rows, right._rows);
				swap(_columns, right._columns);
				swap(_buffer, right._buffer);
			}
		}
	};

	/**
	 * Base matrix class for dynamic matrices only. It defines common constructors and member functions for dynamic
	 * matrices
	 */
	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	class matrix_dynamic_base : public matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>
	{
		using base = matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>;

	protected:
		template<typename... Args>
		matrix_dynamic_base(std::size_t rows, std::size_t columns, Args&&... args) :
			base(rows, columns, std::forward<Args>(args)...)
		{
		}

		matrix_dynamic_base(std::size_t rows,
			std::size_t columns,
			identity_tag,
			const Value& zero_value,
			const Value& one_value,
			const Allocator& allocator) :
			base(rows, columns, rows * columns, zero_value, allocator) // @TODO: ISSUE #20
		{
			base::_rows    = rows;
			base::_columns = columns;

			validate_dimensions_square(*this);
			validate_dimensions_for_identity_matrix(*this);

			make_identity_buffer(base::_buffer, rows, one_value);
		}

		void initialize_from_expression_unchecked(std::size_t rows,
			std::size_t columns,
			const auto& expr) // @TODO: ISSUE #20
		{
			base::_rows    = rows;
			base::_columns = columns;

			base::_buffer.reserve(rows * columns);

			for (auto row = std::size_t{}; row < base::_rows; ++row)
			{
				for (auto column = std::size_t{}; column < base::_columns; ++column)
				{
					base::_buffer.push_back(expr(row, column));
				}
			}
		}

		template<typename Callable>
		void initialize_buffer_from_callable_unchecked(std::size_t rows,
			std::size_t columns,
			Callable&& callable) // @TODO: ISSUE #20
		{
			const auto total_size = rows * columns;

			base::_buffer.reserve(total_size);

			for (auto index = std::size_t{}; index < total_size; ++index)
			{
				base::_buffer.push_back(std::invoke(std::forward<Callable>(callable)));
			}
		}

	public:
		using allocator_type = Allocator;

		using base::operator=;

		[[nodiscard]] auto get_allocator() const -> allocator_type // @TODO: ISSUE #20
		{
			return base::_buffer.get_allocator();
		}

		void clear() // @TODO: ISSUE #20
		{
			base::_rows    = 0;
			base::_columns = 0;
			base::_buffer.clear();
		}
	};
} // namespace mpp::detail
