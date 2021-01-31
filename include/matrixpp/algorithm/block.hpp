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

#include <algorithm>
#include <cstddef>
#include <matrixpp/detail/matrix_base.hpp>
#include <matrixpp/detail/tag_invoke.hpp>
#include <matrixpp/detail/utility.hpp>
#include <matrixpp/matrix.hpp>
#include <span>
#include <stdexcept>

namespace matrixpp
{
	struct block_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(block_t,
			const matrix<Value, RowsExtent, ColumnsExtent>& obj,
			std::size_t top_row_idx,
			std::size_t top_column_idx,
			std::size_t bottom_row_idx,
			std::size_t bottom_column_idx)
			-> matrix<Value, std::dynamic_extent, std::dynamic_extent> // @TODO: ISSUE #20
		{
			// The result matrix is always dynamic because function parameters are always
			// treated as runtime expressions

			// Out of bounds checks

			auto rows      = obj.rows();
			auto cols      = obj.columns();
			auto obj_begin = obj.begin();

			if (top_row_idx >= rows)
			{
				throw std::invalid_argument("Top row index out of bounds!");
			}
			else if (top_column_idx >= cols)
			{
				throw std::invalid_argument("Top column index out of bounds!");
			}
			else if (bottom_row_idx >= rows)
			{
				throw std::invalid_argument("Bottom row index out of bounds!");
			}
			else if (bottom_column_idx >= cols)
			{
				throw std::invalid_argument("Bottom column index out of bounds!");
			}

			// Overlapping checks

			if (top_row_idx > bottom_row_idx)
			{
				throw std::invalid_argument("Top row index bigger than bottom row index!");
			}
			else if (top_column_idx > bottom_column_idx)
			{
				throw std::invalid_argument("Top column index bigger than bottom column index!");
			}

			auto block_matrix            = matrix<Value, std::dynamic_extent, std::dynamic_extent>{};
			auto block_buf               = std::vector<Value>{};
			auto block_buf_back_inserter = std::back_inserter(block_buf);
			auto block_rows              = bottom_row_idx - top_row_idx + 1;
			auto block_cols              = bottom_column_idx - top_column_idx + 1;

			block_buf.reserve(block_rows * block_cols);

			using diff_t = typename matrix<Value, RowsExtent, ColumnsExtent>::difference_type;

			for (auto row = top_row_idx; row <= bottom_row_idx; ++row)
			{
				auto row_begin_idx = static_cast<diff_t>(detail::idx_2d_to_1d(cols, row, top_column_idx));
				auto row_begin     = std::next(obj_begin, row_begin_idx);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_cols), block_buf_back_inserter);
			}

			detail::init_matrix_with_1d_rng_move(block_matrix, std::move(block_buf), block_rows, block_cols);

			return block_matrix;
		}

		template<typename... Args>
		[[nodiscard]] auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<block_t, Args...> // @TODO: ISSUE #20
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto block = block_t{};
} // namespace matrixpp