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
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/matrix.hpp>

#include <cstddef>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		template<typename TransposeAllocator,
			typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename... Args>
		[[nodiscard]] auto trps_impl(const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			[[maybe_unused]] const Args&... alloc_args) -> matrix<Value, ColumnsExtent, RowsExtent, TransposeAllocator>
		{
			const auto rows    = obj.rows();
			const auto columns = obj.columns();
			const auto data    = obj.data();

			using trps_mat_t = matrix<Value, ColumnsExtent, RowsExtent, TransposeAllocator>;
			using trps_buf_t = typename trps_mat_t::buffer_type;

			auto transposed_buffer = [&]() {
				if constexpr (any_extent_is_dynamic(RowsExtent, ColumnsExtent))
				{
					return trps_buf_t{ alloc_args... };
				}
				else
				{
					return trps_buf_t{};
				}
			}();
			detail::allocate_buffer_if_vector(transposed_buffer, columns, rows, Value{});

			for (auto column = std::size_t{}; column < columns; ++column)
			{
				for (auto row = std::size_t{}; row < rows; ++row)
				{
					auto normal_index     = detail::index_2d_to_1d(columns, row, column);
					auto transposed_index = detail::index_2d_to_1d(rows, column, row);

					transposed_buffer[transposed_index] = data[normal_index];
				}
			}

			return [&]() {
				if constexpr (any_extent_is_dynamic(RowsExtent, ColumnsExtent))
				{
					return trps_mat_t{ columns, rows, std::move(transposed_buffer), unsafe, alloc_args... };
				}
				else
				{
					return trps_mat_t{ columns, rows, std::move(transposed_buffer), unsafe };
				}
			}();
		}
	} // namespace detail

	struct transpose_t : public detail::cpo_base<transpose_t>
	{
		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename TransposeAllocator = Allocator>
		[[nodiscard]] friend inline auto tag_invoke(transpose_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			const TransposeAllocator& trps_alloc = TransposeAllocator{})
			-> matrix<Value, ColumnsExtent, RowsExtent, TransposeAllocator> // @TODO: ISSUE #20
		{
			return detail::trps_impl<TransposeAllocator>(obj, trps_alloc);
		}
	};

	inline constexpr auto transpose = transpose_t{};
} // namespace mpp
