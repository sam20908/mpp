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
#include <mpp/detail/expr_base.hpp>
#include <mpp/detail/matrix_iterator.hpp>
#include <mpp/detail/public_tags.hpp>
#include <mpp/detail/utility.hpp>
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
	template<typename Buffer, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
	class matrix_base :
		public expr_base<matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>,
			typename Buffer::value_type,
			RowsExtent,
			ColumnsExtent>,
		public traits<Value>
	{
	protected:
		Buffer _buffer; // Don't default initailize because custom types might not be DefaultConstructible
		std::size_t _rows;
		std::size_t _columns;

		// No point adding this to overload resolution since all matrices implement their own default constructor. The
		// only thing we need this for is static matrices
		matrix_base() = default;

		matrix_base(std::size_t rows, std::size_t columns, const Value& value, const Allocator& allocator) :
			_buffer{ rows * columns, value, allocator }, // @TODO: ISSUE #20
			_rows{ rows },
			_columns{ columns }
		{
		}

		matrix_base(std::size_t rows, std::size_t columns, identity_matrix_tag, const Allocator& allocator) :
			_buffer{ rows * columns, Value{}, allocator }, // @TODO: ISSUE #20
			_rows{ rows },
			_columns{ columns }
		{
			if (rows == 0 || columns == 0)
			{
				throw std::invalid_argument("Identity matrix cannot have a rank of 0!");
			}

			if (rows != columns)
			{
				throw std::invalid_argument("Identity matrix must be square!");
			}

			transform_1d_buffer_into_identity<Value>(_buffer, rows);
		}

		matrix_base(std::size_t rows, std::size_t columns, const Allocator& allocator) :
			_buffer{ allocator }, // @TODO: ISSUE #20
			_rows{ rows },
			_columns{ columns }
		{
		}

		explicit matrix_base(const Allocator& allocator) :
			_buffer{ allocator }, // @TODO: ISSUE #20
			_rows{ 0 },
			_columns{ 0 }
		{
		}

		matrix_base(const matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>& right,
			const Allocator& allocator) :
			_buffer{ right._buffer, allocator }, // @TODO: ISSUE #20
			_rows{ right._rows },
			_columns{ right._columns }
		{
		}

		matrix_base(matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>&& right,
			const Allocator& allocator) :
			_buffer{ std::move(right._buffer), allocator }, // @TODO: ISSUE #20
			_rows{ std::move(right._rows) },
			_columns{ std::move(right._columns) }
		{
		}

		void init_dimension_with_val_static(const Value& value) // @TODO: ISSUE #20
		{
			_rows    = RowsExtent;
			_columns = ColumnsExtent;

			std::ranges::fill(_buffer, value);
		}

		template<typename Range2D>
		void init_buffer_2d_static(Range2D&& range_2d, bool check_range_size) // @TODO: ISSUE #20
		{
			const auto [range_rows, range_columns] = range_2d_dimensions(std::forward<Range2D>(range_2d));

			if (check_range_size && (range_rows != RowsExtent || range_columns != ColumnsExtent))
			{
				throw std::invalid_argument("Extents of static matrix and dimensions of initializer does not match!");
			}

			_rows    = RowsExtent;
			_columns = ColumnsExtent;

			// Keeps track of the beginning of the current row in the 1D buffer where it's empty
			auto row_begin = _buffer.begin();

			if constexpr (std::is_rvalue_reference_v<decltype(range_2d)>)
			{
				for (auto&& row : range_2d)
				{
					std::ranges::move(row, row_begin);
					row_begin += static_cast<difference_type>(range_columns);
				}
			}
			else
			{
				for (auto&& row : range_2d)
				{
					std::ranges::copy(row, row_begin);
					row_begin += static_cast<difference_type>(range_columns);
				}
			}
		}

		void
		init_buffer_2d_dynamic_without_check(const auto& range_2d, std::size_t rows, std::size_t columns) // @TODO: ISSUE #20
		{
			_rows    = rows;
			_columns = columns;
			_buffer.reserve(rows * columns);

			const auto buffer_back_inserter = std::back_inserter(_buffer);

			if constexpr (std::is_rvalue_reference_v<decltype(range_2d)>)
			{
				for (auto&& row : range_2d)
				{
					std::ranges::move(row, buffer_back_inserter);
				}
			}
			else
			{
				for (auto&& row : range_2d)
				{
					std::ranges::copy(row, buffer_back_inserter);
				}
			}
		}

		template<typename Range2D>
		void assign_buffer_2d(Range2D&& range_2d, bool check_range_size) // @TODO: ISSUE #20
		{
			if constexpr (!is_vector<Buffer>::value)
			{
				init_buffer_2d_static(std::forward<Range2D>(range_2d), check_range_size);
			}
			else
			{
				// Since this method is called only after construction, it is safe to use initialized _rows and _columns

				const auto [range_rows, range_columns] = range_2d_dimensions(std::forward<Range2D>(range_2d));

				if (check_range_size && (range_rows != _rows || range_columns != _columns))
				{
					throw std::invalid_argument("Dimensions of matrix and dimensions of initializer does not match!");
				}

				auto begin = _buffer.begin();

				if constexpr (std::is_rvalue_reference_v<decltype(range_2d)>)
				{
					for (auto&& row : range_2d)
					{
						std::ranges::move(row, begin);
						begin += static_cast<difference_type>(_columns);
					}
				}
				else
				{
					for (auto&& row : range_2d)
					{
						std::ranges::copy(row, begin);
						begin += static_cast<difference_type>(_columns);
					}
				}
			}
		}

		void init_expr_dynamic_without_check(std::size_t rows,
			std::size_t columns,
			const auto& expr) // @TODO: ISSUE #20
		{
			_rows    = rows;
			_columns = columns;

			_buffer.reserve(rows * columns);

			for (auto row = std::size_t{}; row < _rows; ++row)
			{
				for (auto col = std::size_t{}; col < _columns; ++col)
				{
					_buffer.push_back(expr(row, col));
				}
			}
		}

		template<typename Callable>
		void
		init_buffer_from_callable_dynamic(std::size_t rows, std::size_t columns, Callable&& callable) // @TODO: ISSUE #20
		{
			_rows    = rows;
			_columns = columns;

			reserve_1d_buffer_if_vector(_buffer, rows, columns);

			const auto total_size = rows * columns;
			for (auto idx = std::size_t{}; idx < total_size; ++idx)
			{
				_buffer.push_back(std::invoke(std::forward<Callable>(callable)));
			}
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

		[[nodiscard]] auto at(std::size_t row_idx, std::size_t col_idx) const -> const_reference // @TODO: ISSUE #20
		{
			if (row_idx >= _rows || col_idx >= _columns)
			{
				throw std::out_of_range("Access out of bounds!");
			}

			return operator()(row_idx, col_idx);
		}

		[[nodiscard]] auto operator()(std::size_t row_idx, std::size_t col_idx) -> reference // @TODO: ISSUE #20
		{
			const auto idx = idx_2d_to_1d(_columns, row_idx, col_idx);

			return _buffer[idx];
		}

		[[nodiscard]] auto operator()(std::size_t row_idx,
			std::size_t col_idx) const -> const_reference // @TODO: ISSUE #20
		{
			const auto idx = idx_2d_to_1d(_columns, row_idx, col_idx);

			return _buffer[idx];
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

		// Note that size of the buffer isn't an exact representation of the size of the matrix because we store the
		// matrix in 1D, but due to Standard conformance, we have to provide size() and max_size()

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

		void assign(std::initializer_list<std::initializer_list<Value>> init_2d) // @TODO: ISSUE #20
		{
			assign_buffer_2d(init_2d, true);
		}

		template<range_2d_with_type<Value> Range2D>
		void assign(Range2D&& range_2d) // @TODO: ISSUE #20
		{
			assign_buffer_2d(std::forward<Range2D>(range_2d), true);
		}

		auto operator=(std::initializer_list<std::initializer_list<Value>> init_2d) -> matrix_base& // @TODO: ISSUE #20
		{
			assign_buffer_2d(init_2d, true);
			return *this;
		}

		template<range_2d_with_type<Value> Range2D>
		auto operator=(Range2D&& range_2d) -> matrix_base& // @TODO: ISSUE #20
		{
			assign_buffer_2d(std::forward<Range2D>(range_2d), true);
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
			matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>& base,
			const auto& range,
			std::size_t rows,
			std::size_t columns) // @TODO: ISSUE #20
		{
			base._rows    = rows;
			base._columns = columns;

			reserve_1d_buffer_if_vector(base._buffer, rows, columns);

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
					auto idx = std::size_t{};

					for (auto&& val : range)
					{
						base._buffer[idx++] = std::move(static_cast<Value>(val));
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
					auto idx = std::size_t{};

					for (const auto& val : range)
					{
						base._buffer[idx++] = static_cast<Value>(val);
					}
				}
			}
		}
	};

	/**
	 * Base matrix class for dynamic matrices only. It defines common constructors and member functions for dynamic
	 * matrices
	 */
	template<typename Buffer, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
	class matrix_dynamic_base : public matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>
	{
		using base = matrix_base<Buffer, Value, RowsExtent, ColumnsExtent, Allocator>;

	protected:
		explicit matrix_dynamic_base(std::size_t rows,
			std::size_t columns,
			const Value& value,
			const Allocator& allocator) :
			base(rows, columns, value, allocator) // @TODO: ISSUE #20
		{
		}

		explicit matrix_dynamic_base(std::size_t rows,
			std::size_t columns,
			identity_matrix_tag,
			const Allocator& allocator) :
			base(rows, columns, identity_matrix_tag{}, allocator) // @TODO: ISSUE #20
		{
		}

		explicit matrix_dynamic_base(std::size_t rows, std::size_t columns, const Allocator& allocator) :
			base(rows, columns, Value{}, allocator) // @TODO: ISSUE #20
		{
		}

		matrix_dynamic_base(const matrix_dynamic_base& right, const Allocator& allocator) :
			base(right, allocator) // @TODO: ISSUE #20
		{
		}

		matrix_dynamic_base(matrix_dynamic_base&& right, const Allocator& allocator) :
			base(std::move(right), allocator) // @TODO: ISSUE #20
		{
		}

		explicit matrix_dynamic_base(const Allocator& allocator) : base(allocator) {} // @TODO: ISSUE #20

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
