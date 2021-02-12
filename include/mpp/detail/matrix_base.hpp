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
#include <mpp/detail/utility.hpp>
#include <mpp/utility/traits.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace mpp::detail
{
	/**
	 * Base matrix class to store internal data and define common member functions
	 */
	template<typename Buffer, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	class matrix_base :
		public expr_base<matrix_base<Buffer, Value, RowsExtent, ColumnsExtent>,
			typename Buffer::value_type,
			RowsExtent,
			ColumnsExtent>,
		public traits<Value>
	{
	protected:
		Buffer _buf; // Don't default initailize because custom types might not be DefaultConstructible
		std::size_t _rows;
		std::size_t _cols;

		void init_buf_2d_static(auto&& rng_2d, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			_rows = rows;
			_cols = cols;

			// Keeps track of the beginning of the current row in the 1D buffer where it's empty
			auto row_begin = _buf.begin();

			if constexpr (std::is_rvalue_reference_v<decltype(rng_2d)>)
			{
				for (auto&& row : rng_2d)
				{
					std::ranges::move(row, row_begin);
					row_begin += cols;
				}
			}
			else
			{
				for (auto&& row : rng_2d)
				{
					std::ranges::copy(row, row_begin);
					row_begin += cols;
				}
			}
		}

		void init_buf_2d_dynamic(auto&& rng_2d, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			_rows = rows;
			_cols = cols;
			_buf.reserve(rows * cols);

			const auto buf_back_inserter = std::back_inserter(_buf);

			if constexpr (std::is_rvalue_reference_v<decltype(rng_2d)>)
			{
				for (auto&& row : rng_2d)
				{
					std::ranges::move(row, buf_back_inserter);
				}
			}
			else
			{
				for (auto&& row : rng_2d)
				{
					std::ranges::copy(row, buf_back_inserter);
				}
			}
		}

		void init_expr_dynamic(std::size_t rows, std::size_t cols, auto&& expr, bool check_size)
		{
			_rows = rows;
			_cols = cols;

			if (check_size)
			{
				validate_same_size(*this, expr);
			}

			_buf.reserve(rows * cols);

			for (auto row = std::size_t{}; row < _rows; ++row)
			{
				for (auto col = std::size_t{}; col < _cols; ++col)
				{
					_buf.push_back(expr(row, col));
				}
			}
		}

		void init_identity(std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			if (rows == 0 || cols == 0)
			{
				throw std::invalid_argument("Identity matrix cannot have a rank of 0!");
			}

			if (rows != cols)
			{
				throw std::invalid_argument("Identity matrix must be square!");
			}

			_rows = rows;
			_cols = cols;

			allocate_1d_buf_if_vector(_buf, rows, cols, Value{});
			transform_1d_buf_into_identity<Value>(_buf, rows);
		}

	public:
		using buffer_type     = Buffer;
		using value_type      = Value;
		using difference_type = typename buffer_type::difference_type;
		using pointer         = typename buffer_type::pointer;
		using const_pointer   = typename buffer_type::const_pointer;
		using iterator        = typename buffer_type::iterator;
		using const_iterator  = typename buffer_type::const_iterator;

		[[nodiscard]] auto data() -> pointer // @TODO: ISSUE #20
		{
			return _buf.data();
		}

		[[nodiscard]] auto data() const -> const_pointer // @TODO: ISSUE #20
		{
			return _buf.data();
		}

		[[nodiscard]] auto begin() -> iterator // @TODO: ISSUE #20, #21
		{
			return _buf.begin();
		}

		[[nodiscard]] auto begin() const -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cbegin();
		}

		[[nodiscard]] auto end() -> iterator // @TODO: ISSUE #20, #21
		{
			return _buf.end();
		}

		[[nodiscard]] auto end() const -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cend();
		}

		[[nodiscard]] auto cbegin() -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cbegin();
		}

		[[nodiscard]] auto cbegin() const -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cbegin();
		}

		[[nodiscard]] auto cend() -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cend();
		}

		[[nodiscard]] auto cend() const -> const_iterator // @TODO: ISSUE #20, #21
		{
			return _buf.cend();
		}

		[[nodiscard]] auto at(std::size_t row_idx, std::size_t col_idx) const -> const value_type& // @TODO: ISSUE #20
		{
			if (row_idx >= _rows || col_idx >= _cols)
			{
				throw std::out_of_range("Access out of bounds!");
			}

			return operator()(row_idx, col_idx);
		}

		[[nodiscard]] auto operator()(std::size_t row_idx, std::size_t col_idx) -> value_type& // @TODO: ISSUE #20
		{
			const auto idx = idx_2d_to_1d(_cols, row_idx, col_idx);

			return _buf[idx];
		}

		[[nodiscard]] auto operator()(std::size_t row_idx,
			std::size_t col_idx) const -> const value_type& // @TODO: ISSUE #20
		{
			const auto idx = idx_2d_to_1d(_cols, row_idx, col_idx);

			return _buf[idx];
		}

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return _rows;
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return _cols;
		}

		friend inline void init_matrix_with_1d_rng(matrix_base<Buffer, Value, RowsExtent, ColumnsExtent>& base,
			auto&& rng,
			std::size_t rows,
			std::size_t cols) // @TODO: ISSUE #20
		{
			base._rows = rows;
			base._cols = cols;

			reserve_1d_buf_if_vector(base._buf, rows, cols);

			if constexpr (std::is_rvalue_reference_v<decltype(rng)>)
			{
				if constexpr (is_vector<Buffer>::value)
				{
					std::ranges::move(rng, std::back_inserter(base._buf));
				}
				else
				{
					std::ranges::move(rng, base._buf.begin());
				}
			}
			else
			{
				if constexpr (is_vector<Buffer>::value)
				{
					std::ranges::copy(rng, std::back_inserter(base._buf));
				}
				else
				{
					std::ranges::copy(rng, base._buf.begin());
				}
			}
		}
	};
} // namespace mpp::detail