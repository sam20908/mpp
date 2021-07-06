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
#include <mpp/detail/utility/utility.hpp>
#include <mpp/matrix.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		constexpr void assert_valid_block_dims(std::size_t rows,
			std::size_t columns,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index)
		{
			// Out of bounds asserts

			assert(top_row_index < rows);
			assert(top_column_index < columns);
			assert(bottom_row_index < rows);
			assert(bottom_column_index < columns);

			// Overlapping asserts

			assert(top_row_index <= bottom_row_index);
			assert(top_column_index <= bottom_column_index);
		}

		template<typename Val,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			std::size_t TopRowIndex,
			std::size_t TopColumnIndex,
			std::size_t BottomRowIndex,
			std::size_t BottomColumnIndex,
			typename... Alloc>
		using block_mat_ret_t = matrix<Val,
			any_extent_is_dynamic(RowsExtent, TopRowIndex, BottomRowIndex) ? dynamic : BottomRowIndex - TopRowIndex + 1,
			any_extent_is_dynamic(ColumnsExtent, TopColumnIndex, BottomColumnIndex)
				? dynamic
				: BottomColumnIndex - TopColumnIndex + 1,
			Alloc...>;

		template<typename BlockMat>
		auto block_impl_fixed(const auto& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index) -> BlockMat // @TODO: ISSUE #20
		{
			using diff_t = typename BlockMat::difference_type;
			using buf_t  = typename BlockMat::buffer_type;

			auto buf = buf_t{};

			auto buf_begin = buf.begin();
			auto obj_begin = obj.begin();

			const auto columns       = obj.columns();
			const auto block_rows    = bottom_row_index - top_row_index + 1;
			const auto block_columns = bottom_column_index - top_column_index + 1;

			for (auto row = top_row_index; row <= bottom_row_index; ++row)
			{
				auto row_begin_index = static_cast<diff_t>(index_2d_to_1d(columns, row, top_column_index));
				auto row_begin       = std::next(obj_begin, row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_columns), buf_begin);

				std::advance(buf_begin, block_columns);
			}

			return BlockMat{ block_rows, block_columns, std::move(buf) };
		}

		template<typename BlockMat, typename... Args>
		auto block_impl_dyn(const auto& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index,
			const Args&... alloc_args) -> BlockMat // @TODO: ISSUE #20
		{
			using diff_t = typename BlockMat::difference_type;
			using buf_t  = typename BlockMat::buffer_type;

			const auto columns       = obj.columns();
			const auto block_rows    = bottom_row_index - top_row_index + 1;
			const auto block_columns = bottom_column_index - top_column_index + 1;

			auto buf = buf_t{ alloc_args... };
			buf.reserve(block_rows * block_columns);

			auto inserter = std::back_inserter(buf);

			for (auto row = top_row_index; row <= bottom_row_index; ++row)
			{
				auto row_begin_index = static_cast<diff_t>(index_2d_to_1d(columns, row, top_column_index));
				auto row_begin       = std::next(obj.begin(), row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_columns), inserter);
			}

			return BlockMat{ block_rows, block_columns, std::move(buf), alloc_args... };
		}

		template<typename BlockAllocator,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename TopRowIndex,
			typename TopColumnIndex,
			typename BottomRowIndex,
			typename BottomColumnIndex,
			typename Allocator,
			typename... Args>
		[[nodiscard]] inline auto block_impl(const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			TopRowIndex top_row_index,
			TopColumnIndex top_column_index,
			BottomRowIndex bottom_row_index,
			BottomColumnIndex bottom_column_index,
			const Args&... alloc_args) -> block_mat_ret_t<Value,
			RowsExtent,
			ColumnsExtent,
			get_constant_val_or_dynamic<TopRowIndex>(),
			get_constant_val_or_dynamic<TopColumnIndex>(),
			get_constant_val_or_dynamic<BottomRowIndex>(),
			get_constant_val_or_dynamic<BottomColumnIndex>(),
			BlockAllocator> // @TODO: ISSUE #20
		{
			// The static_cast are to trigger the explicit conversion operator for mpp::constant objects

			assert_valid_block_dims(obj.rows(),
				obj.columns(),
				static_cast<std::size_t>(top_row_index),
				static_cast<std::size_t>(top_column_index),
				static_cast<std::size_t>(bottom_row_index),
				static_cast<std::size_t>(bottom_column_index));

			using block_mat_t = block_mat_ret_t<Value,
				RowsExtent,
				ColumnsExtent,
				get_constant_val_or_dynamic<TopRowIndex>(),
				get_constant_val_or_dynamic<TopColumnIndex>(),
				get_constant_val_or_dynamic<BottomRowIndex>(),
				get_constant_val_or_dynamic<BottomColumnIndex>(),
				BlockAllocator>;

			constexpr auto buf_is_vec = any_extent_is_dynamic(RowsExtent, ColumnsExtent);

			if constexpr (buf_is_vec)
			{
				return block_impl_dyn<block_mat_t>(obj,
					static_cast<std::size_t>(top_row_index),
					static_cast<std::size_t>(top_column_index),
					static_cast<std::size_t>(bottom_row_index),
					static_cast<std::size_t>(bottom_column_index),
					alloc_args...);
			}
			else
			{
				return block_impl_fixed<block_mat_t>(obj,
					static_cast<std::size_t>(top_row_index),
					static_cast<std::size_t>(top_column_index),
					static_cast<std::size_t>(bottom_row_index),
					static_cast<std::size_t>(bottom_column_index));
			}
		}
	} // namespace detail

	struct block_t : public detail::cpo_base<block_t>
	{
		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			detail::constant_or_convertible_to_size_t TopRowIndex,
			detail::constant_or_convertible_to_size_t TopColumnIndex,
			detail::constant_or_convertible_to_size_t BottomRowIndex,
			detail::constant_or_convertible_to_size_t BottomColumnIndex,
			typename Allocator,
			typename BlockAllocator = Allocator>
		[[nodiscard]] friend inline auto tag_invoke(block_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			TopRowIndex top_row_index,
			TopColumnIndex top_column_index,
			BottomRowIndex bottom_row_index,
			BottomColumnIndex bottom_column_index,
			const BlockAllocator& block_alloc = BlockAllocator{}) -> detail::block_mat_ret_t<Value,
			RowsExtent,
			ColumnsExtent,
			detail::get_constant_val_or_dynamic<TopRowIndex>(),
			detail::get_constant_val_or_dynamic<TopColumnIndex>(),
			detail::get_constant_val_or_dynamic<BottomRowIndex>(),
			detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
			BlockAllocator> // @TODO: ISSUE #20
		{
			return detail::block_impl<BlockAllocator>(obj,
				top_row_index,
				top_column_index,
				bottom_row_index,
				bottom_column_index,
				block_alloc);
		}
	};

	inline constexpr auto block = block_t{};
} // namespace mpp
