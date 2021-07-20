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

#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/mat/matfwd.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		constexpr void assert_valid_block_dims(std::size_t rows,
			std::size_t cols,
			std::size_t top_row_idx,
			std::size_t top_col_idx,
			std::size_t bottom_row_idx,
			std::size_t bottom_col_idx)
		{
			// Out of bounds asserts

			assert(top_row_idx < rows);
			assert(top_col_idx < cols);
			assert(bottom_row_idx < rows);
			assert(bottom_col_idx < cols);

			// Overlapping asserts

			assert(top_row_idx <= bottom_row_idx);
			assert(top_col_idx <= bottom_col_idx);
		}

		template<typename To>
		auto block_impl_fixed(const auto& obj,
			std::size_t top_row_idx,
			std::size_t top_col_idx,
			std::size_t bottom_row_idx,
			std::size_t bottom_col_idx) -> To // @TODO: ISSUE #20
		{
			using diff_t = typename To::difference_type;
			using buf_t  = typename To::buffer_type;

			auto buf = buf_t{};

			auto buf_begin = buf.begin();
			auto obj_begin = obj.begin();

			const auto cols       = obj.cols();
			const auto block_rows = bottom_row_idx - top_row_idx + 1;
			const auto block_cols = bottom_col_idx - top_col_idx + 1;

			for (auto row = top_row_idx; row <= bottom_row_idx; ++row)
			{
				auto row_begin_index = static_cast<diff_t>(idx_1d(cols, row, top_col_idx));
				auto row_begin       = std::next(obj_begin, row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_cols), buf_begin);

				std::advance(buf_begin, block_cols);
			}

			return To{ block_rows, block_cols, std::move(buf) };
		}

		template<typename To>
		auto block_impl_dyn(const auto& obj,
			std::size_t top_row_idx,
			std::size_t top_col_idx,
			std::size_t bottom_row_idx,
			std::size_t bottom_col_idx) -> To // @TODO: ISSUE #20
		{
			using diff_t = typename To::difference_type;
			using buf_t  = typename To::buffer_type;

			const auto cols       = obj.cols();
			const auto block_rows = bottom_row_idx - top_row_idx + 1;
			const auto block_cols = bottom_col_idx - top_col_idx + 1;

			auto buf = buf_t{};
			buf.reserve(block_rows * block_cols);

			auto inserter = std::back_inserter(buf);

			for (auto row = top_row_idx; row <= bottom_row_idx; ++row)
			{
				auto row_begin_index = static_cast<diff_t>(idx_1d(cols, row, top_col_idx));
				auto row_begin       = std::next(obj.begin(), row_begin_index);

				std::ranges::copy_n(row_begin, static_cast<diff_t>(block_cols), inserter);
			}

			return To{ block_rows, block_cols, std::move(buf) };
		}

		template<typename To>
		[[nodiscard]] inline auto block_impl(const auto& obj,
			auto top_row_idx,
			auto top_col_idx,
			auto bottom_row_idx,
			auto bottom_col_idx) -> To // @TODO: ISSUE #20
		{
			assert_valid_block_dims(obj.rows(), obj.cols(), top_row_idx, top_col_idx, bottom_row_idx, bottom_col_idx);

			if constexpr (is_vec<typename To::buffer_type>::value)
			{
				return block_impl_dyn<To>(obj, top_row_idx, top_col_idx, bottom_row_idx, bottom_col_idx);
			}
			else
			{
				return block_impl_fixed<To>(obj, top_row_idx, top_col_idx, bottom_row_idx, bottom_col_idx);
			}
		}
	} // namespace detail

	struct block_t : public detail::cpo_base<block_t>
	{
		template<typename Val,
			std::size_t Rows,
			std::size_t Cols,
			typename Alloc,
			typename To = mat<Val, dyn, dyn, Alloc>>
		requires(detail::is_matrix<To>::value) [[nodiscard]] friend inline auto tag_invoke(block_t,
			const mat<Val, Rows, Cols, Alloc>& obj,
			std::size_t top_row_idx,
			std::size_t top_col_idx,
			std::size_t bottom_row_idx,
			std::size_t bottom_col_idx,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::block_impl<To>(obj, top_row_idx, top_col_idx, bottom_row_idx, bottom_col_idx);
		}
	};

	inline constexpr auto block = block_t{};
} // namespace mpp
