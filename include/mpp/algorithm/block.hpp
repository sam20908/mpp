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
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/detail/utility/exception_messages.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/matrix.hpp>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace mpp
{
	namespace detail
	{
		// Don't put this in validators.hpp as it's only used here
		constexpr void validate_block_index_boundaries(std::size_t rows,
			std::size_t columns,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index)
		{
			// Out of bounds checks

			if (top_row_index >= rows)
			{
				throw std::invalid_argument(BLOCK_TOP_ROW_INDEX_OUT_OF_BOUNDS);
			}
			else if (top_column_index >= columns)
			{
				throw std::invalid_argument(BLOCK_TOP_COLUMN_INDEX_OUT_OF_BOUNDS);
			}
			else if (bottom_row_index >= rows)
			{
				throw std::invalid_argument(BLOCK_BOTTOM_ROW_INDEX_OUT_OF_BOUNDS);
			}
			else if (bottom_column_index >= columns)
			{
				throw std::invalid_argument(BLOCK_BOTTOM_COLUMN_INDEX_OUT_OF_BOUNDS);
			}

			// Overlapping checks

			if (top_row_index > bottom_row_index)
			{
				throw std::invalid_argument(BLOCK_TOP_ROW_INDEX_BIGGER_THAN_BOTTOM_ROW_INDEX);
			}
			else if (top_column_index > bottom_column_index)
			{
				throw std::invalid_argument(BLOCK_TOP_COLUMN_INDEX_BIGGER_THAN_BOTTOM_COLUMN_INDEX);
			}
		}
	} // namespace detail

	struct block_t : public detail::cpo_base<block_t>
	{
		// @TODO: Support fixed block matrix with certain preconditions (#225)

		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(block_t,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index) -> matrix<Value, dynamic, dynamic> // @TODO: ISSUE #20
		{
			// The result matrix is always dynamic because function parameters are always treated as runtime
			// expressions, which means it's impossible to change the extent to resized extent

			const auto rows    = obj.rows();
			const auto columns = obj.columns();

			detail::validate_block_index_boundaries(rows,
				columns,
				top_row_index,
				top_column_index,
				bottom_row_index,
				bottom_column_index);

			return tag_invoke(block_t{},
				obj,
				top_row_index,
				top_column_index,
				bottom_row_index,
				bottom_column_index,
				unsafe);
		}

		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(block_t,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index,
			unsafe_tag) -> matrix<Value, dynamic, dynamic> // @TODO: ISSUE #20
		{
			const auto columns = obj.columns();
			const auto begin   = obj.begin();

			using block_matrix_t  = matrix<Value, dynamic, dynamic>;
			using block_buffer_t  = typename block_matrix_t::buffer_type;
			using difference_type = typename block_matrix_t::difference_type;

			auto block_buffer               = block_buffer_t{};
			auto block_buffer_back_inserter = std::back_inserter(block_buffer);

			const auto block_rows    = bottom_row_index - top_row_index + 1;
			const auto block_columns = bottom_column_index - top_column_index + 1;
			block_buffer.reserve(block_rows * block_columns);

			for (auto row = top_row_index; row <= bottom_row_index; ++row)
			{
				auto row_begin_index =
					static_cast<difference_type>(detail::index_2d_to_1d(columns, row, top_column_index));
				auto row_begin = std::next(begin, row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<difference_type>(block_columns), block_buffer_back_inserter);
			}

			return block_matrix_t{ block_rows, block_columns, std::move(block_buffer) };
		}
	};

	inline constexpr auto block = block_t{};
} // namespace mpp