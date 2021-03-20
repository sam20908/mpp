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

namespace mpp
{
	struct transpose_t : public detail::cpo_base<transpose_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(transpose_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<Value, ColumnsExtent, RowsExtent> // @TODO: ISSUE #20
		{
			const auto rows    = obj.rows();
			const auto columns = obj.columns();
			const auto data    = obj.data();

			using transposed_t        = matrix<Value, ColumnsExtent, RowsExtent>;
			using transposed_buffer_t = typename transposed_t::buffer_type;
			auto transposed           = transposed_t{};
			auto transposed_buffer    = transposed_buffer_t{};
			detail::allocate_buffer_if_vector(transposed_buffer, columns, rows, Value{});

			for (auto col = std::size_t{}; col < columns; ++col)
			{
				for (auto row = std::size_t{}; row < rows; ++row)
				{
					auto normal_index     = detail::index_2d_to_1d(columns, row, col);
					auto transposed_index = detail::index_2d_to_1d(rows, col, row);

					transposed_buffer[transposed_index] = data[normal_index];
				}
			}

			init_matrix_with_1d_range(transposed, std::move(transposed_buffer), columns, rows);

			return transposed;
		}
	};

	inline constexpr auto transpose = transpose_t{};
} // namespace mpp