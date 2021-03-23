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
#include <mpp/detail/matrix/public_tags.hpp>
#include <mpp/detail/types/constraints.hpp>
#include <mpp/detail/types/type_traits.hpp>
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/detail/utility/validators.hpp>
#include <mpp/utility/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>
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
	class matrix_base :
		public expr_base<matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>,
			Value,
			RowsExtent,
			ColumnsExtent>,
		public traits<Value>
	{
	protected:
		Buffer _buffer; // Don't default initailize because value type might not be DefaultConstructible
		std::size_t _rows{ RowsExtent == std::dynamic_extent ? std::size_t{} : RowsExtent };
		std::size_t _columns{ ColumnsExtent == std::dynamic_extent ? std::size_t{} : ColumnsExtent };

		// This is needed for fully static matrices
		matrix_base() = default;

		template<typename... Args>
		matrix_base(std::size_t rows, std::size_t columns, Args&&... args) :
			_buffer(std::forward<Args>(args)...),
			_rows{ rows },
			_columns{ columns } // @TODO: ISSUE #20
		{
		}

		template<bool CheckAgainstCurrentRows,
			bool CheckAgainstCurrentColumns,
			bool CheckUnequalColumns,
			bool Unsafe,
			typename Range2D>
		void assign_and_insert_if_bigger(Range2D&& range_2d)
		{
			const auto range_rows = std::ranges::size(std::forward<Range2D>(range_2d));

			if constexpr (CheckAgainstCurrentRows)
			{
				if (range_rows != _rows)
				{
					throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
				}
			}

			auto range_begin         = std::ranges::begin(range_2d);
			const auto range_columns = range_rows > 0 ? std::ranges::size(*range_begin) : std::size_t{};

			constexpr auto range_is_moved = std::is_rvalue_reference_v<Range2D>;

			// Assign until min(range_rows, _rows)
			const auto min_rows = (std::min)(range_rows, _rows);
			auto buffer_begin   = _buffer.begin();

			// Handle dynamic column matrices
			if (_columns != 0)
			{
				for (auto row = std::size_t{}; row < min_rows; ++row)
				{
					const auto current_columns = std::ranges::size(*range_begin);

					if constexpr (CheckAgainstCurrentColumns)
					{
						if (current_columns != _columns)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (CheckUnequalColumns)
					{
						if (current_columns != range_columns)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (range_is_moved)
					{
						std::ranges::move(*range_begin, buffer_begin);
					}
					else
					{
						std::ranges::copy(*range_begin, buffer_begin);
					}

					++range_begin;
					buffer_begin += static_cast<difference_type>(range_columns);
				}
			}

			// Insert every element after (only dynamic matrices reach this point)
			if constexpr (is_vector<Buffer>::value)
			{
				_buffer.reserve(range_rows * range_columns);

				const auto buffer_back_inserter = std::back_inserter(_buffer);
				const auto starting_rows        = _columns == 0 ? 0 : _rows; // Handle dynamic column matrices

				for (auto row = starting_rows; row < range_rows; ++row)
				{
					const auto current_columns = std::ranges::size(*range_begin);

					if constexpr (CheckAgainstCurrentColumns)
					{
						if (current_columns != _columns)
						{
							throw std::invalid_argument(detail::INITIALIZER_INCOMPATIBLE_DIMENSION_EXTENTS);
						}
					}

					if constexpr (CheckUnequalColumns)
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

			_rows    = range_rows;
			_columns = range_columns;
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
			Derived::assign_buffer_2d(std::forward<Range2D>(range_2d), true);
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

		friend inline void init_matrix_with_1d_range(
			matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>& base,
			const auto& range,
			std::size_t rows,
			std::size_t columns) // @TODO: ISSUE #20
		{
			base._rows    = rows;
			base._columns = columns;

			reserve_buffer_if_vector(base._buffer, rows, columns);

			// @FIXME: Probably not optimal. Look at this again later
			if constexpr (std::is_rvalue_reference_v<decltype(range)>)
			{
				if constexpr (is_vector<Buffer>::value)
				{
					base._buffer.reserve(std::ranges::size(range));

					for (auto&& val : range)
					{
						base._buffer.push_back(std::move(static_cast<Value>(val)));
					}
				}
				else
				{
					auto index = std::size_t{};

					for (auto&& val : range)
					{
						base._buffer[index++] = std::move(static_cast<Value>(val));
					}
				}
			}
			else
			{
				if constexpr (is_vector<Buffer>::value)
				{
					base._buffer.reserve(std::ranges::size(range));

					for (const auto& val : range)
					{
						base._buffer.push_back(static_cast<Value>(val));
					}
				}
				else
				{
					auto index = std::size_t{};

					for (const auto& val : range)
					{
						base._buffer[index++] = static_cast<Value>(val);
					}
				}
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
			identity_matrix_tag,
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
