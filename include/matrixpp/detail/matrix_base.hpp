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

#include "constraints.hpp"
#include "expr_base.hpp"
#include "utility.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>
#include <type_traits>

namespace matrixpp::detail
{
	/**
     * Base matrix class to store internal data and define common member functions
     */
	template<typename Buffer, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	class matrix_base :
		public expr_base<matrix_base<Buffer, Value, RowsExtent, ColumnsExtent>,
			typename Buffer::value_type,
			RowsExtent,
			ColumnsExtent>
	{
	protected:
		Buffer _buf;
		std::size_t _rows{ RowsExtent == std::dynamic_extent ? 0 : RowsExtent };
		std::size_t _cols{ ColumnsExtent == std::dynamic_extent ? 0 : ColumnsExtent };

		void init_buf_2d_static(auto&& buf_2d, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			_rows = rows;
			_cols = cols;

			auto idx = std::size_t{ 0 };
			for (const auto& row : buf_2d)
			{
				for (auto value : row)
				{
					_buf[idx++] = value;
				}
			}
		}

		void init_buf_2d_dynamic(auto&& buf_2d, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			_rows = rows;
			_cols = cols;
			_buf.reserve(rows * cols);

			for (const auto& row : buf_2d)
			{
				for (auto value : row)
				{
					_buf.push_back(value);
				}
			}
		}

		void init_identity(std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
		{
			if (rows != cols)
			{
				throw std::invalid_argument("Identity matrix must be square!");
			}

			_rows = rows;
			_cols = columns;

			allocate_1d_buf_if_vector(_buf, rows, cols);
			transform_1d_buf_into_identity<Value>(_buf, rows);
		}

	public:
		using buffer_type     = Buffer;
		using value_type      = Value;
		using difference_type = typename buffer_type::difference_type;
		using pointer         = typename buffer_type::pointer;
		using iterator        = typename buffer_type::iterator;
		using const_iterator  = typename buffer_type::const_iterator;

		matrix_base() = default; // @TODO: ISSUE #20

		[[nodiscard]] auto buffer() const -> const buffer_type& // @TODO: ISSUE #20
		{
			return _buf;
		}

		[[nodiscard]] auto data() const -> pointer // @TODO: ISSUE #20
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
			auto index = idx_2d_to_1d(_cols, row_idx, col_idx);

			return _buf[index];
		}

		[[nodiscard]] const auto operator()(std::size_t row_idx,
			std::size_t col_idx) const -> const value_type& // @TODO: ISSUE #20
		{
			auto index = idx_2d_to_1d(_cols, row_idx, col_idx);

			return _buf[index];
		}

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return _rows;
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return _cols;
		}

		[[nodiscard]] constexpr static auto rows_extent() -> std::size_t
		{
			return RowsExtent;
		}

		[[nodiscard]] constexpr static auto columns_extent() -> std::size_t
		{
			return ColumnsExtent;
		}

		template<typename BaseBuffer, typename BaseValue, std::size_t BaseRowsExtent, std::size_t BaseColumnsExtent>
		friend inline void init_matrix_with_1d_rng_copy(
			matrix_base<BaseBuffer, BaseValue, BaseRowsExtent, BaseColumnsExtent>& base,
			detail::range_arithmetic auto&& rng,
			std::size_t rows,
			std::size_t cols); // @TODO: ISSUE #20

		template<typename BaseBuffer, typename BaseValue, std::size_t BaseRowsExtent, std::size_t BaseColumnsExtent>
		friend inline void init_matrix_with_1d_rng_move(
			matrix_base<BaseBuffer, BaseValue, BaseRowsExtent, BaseColumnsExtent>& base,
			detail::range_arithmetic auto&& rng,
			std::size_t rows,
			std::size_t cols); // @TODO: ISSUE #20
	};

	template<typename BaseBuffer, typename BaseValue, std::size_t BaseRowsExtent, std::size_t BaseColumnsExtent>
	inline void init_matrix_with_1d_rng_copy(
		matrix_base<BaseBuffer, BaseValue, BaseRowsExtent, BaseColumnsExtent>& base,
		detail::range_arithmetic auto&& rng,
		std::size_t rows,
		std::size_t cols) // @TODO: ISSUE #20
	{
		base._rows = rows;
		base._cols = cols;
		allocate_1d_buf_if_vector(base._buf, rows, cols);

		std::ranges::copy(rng, base._buf.begin());
	}

	template<typename BaseBuffer, typename BaseValue, std::size_t BaseRowsExtent, std::size_t BaseColumnsExtent>
	inline void init_matrix_with_1d_rng_move(
		matrix_base<BaseBuffer, BaseValue, BaseRowsExtent, BaseColumnsExtent>& base,
		detail::range_arithmetic auto&& rng,
		std::size_t rows,
		std::size_t cols) // @TODO: ISSUE #20
	{
		base._rows = rows;
		base._cols = cols;
		allocate_1d_buf_if_vector(base._buf, rows, cols);

		std::ranges::move(rng, base._buf.begin());
	}
} // namespace matrixpp::detail