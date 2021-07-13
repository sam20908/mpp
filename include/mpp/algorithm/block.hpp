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

		template<typename To>
		auto block_impl_fixed(const auto& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index) -> To // @TODO: ISSUE #20
		{
			using diff_t = typename To::difference_type;
			using buf_t  = typename To::buffer_type;

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

			return To{ block_rows, block_columns, std::move(buf) };
		}

		template<typename To>
		auto block_impl_dyn(const auto& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index) -> To // @TODO: ISSUE #20
		{
			using diff_t = typename To::difference_type;
			using buf_t  = typename To::buffer_type;

			const auto columns       = obj.columns();
			const auto block_rows    = bottom_row_index - top_row_index + 1;
			const auto block_columns = bottom_column_index - top_column_index + 1;

			auto buf = buf_t{};
			buf.reserve(block_rows * block_columns);

			auto inserter = std::back_inserter(buf);

			for (auto row = top_row_index; row <= bottom_row_index; ++row)
			{
				auto row_begin_index = static_cast<diff_t>(index_2d_to_1d(columns, row, top_column_index));
				auto row_begin       = std::next(obj.begin(), row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_columns), inserter);
			}

			return To{ block_rows, block_columns, std::move(buf) };
		}

		template<typename To>
		[[nodiscard]] inline auto block_impl(const auto& obj,
			auto top_row_index,
			auto top_column_index,
			auto bottom_row_index,
			auto bottom_column_index) -> To // @TODO: ISSUE #20
		{
			assert_valid_block_dims(obj.rows(),
				obj.columns(),
				top_row_index,
				top_column_index,
				bottom_row_index,
				bottom_column_index);

			constexpr auto buf_is_vec = is_vector<typename To::buffer_type>::value;

			if constexpr (buf_is_vec)
			{
				return block_impl_dyn<To>(obj, top_row_index, top_column_index, bottom_row_index, bottom_column_index);
			}
			else
			{
				return block_impl_fixed<To>(obj,
					top_row_index,
					top_column_index,
					bottom_row_index,
					bottom_column_index);
			}
		}
	} // namespace detail

	struct block_t : public detail::cpo_base<block_t>
	{
		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename To = matrix<Value, dynamic, dynamic, Allocator>>
		requires(detail::is_matrix<To>::value) [[nodiscard]] friend inline auto tag_invoke(block_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			std::size_t top_row_index,
			std::size_t top_column_index,
			std::size_t bottom_row_index,
			std::size_t bottom_column_index,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::block_impl<To>(obj, top_row_index, top_column_index, bottom_row_index, bottom_column_index);
		}
	};

	inline constexpr auto block = block_t{};
} // namespace mpp
