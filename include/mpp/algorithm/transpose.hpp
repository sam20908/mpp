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

#include <mpp/detail/matrix_base.hpp>
#include <mpp/detail/utility.hpp>
#include <mpp/matrix.hpp>

#include <cstddef>

namespace mpp
{
	struct transpose_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(transpose_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<Value, ColumnsExtent, RowsExtent> // @TODO: ISSUE #20
		{
			const auto rows = obj.rows();
			const auto cols = obj.columns();
			const auto data = obj.data();

			using transposed_t     = matrix<Value, ColumnsExtent, RowsExtent>;
			using transposed_buf_t = typename transposed_t::buffer_type;
			auto transposed        = transposed_t{};
			auto transposed_buf    = transposed_buf_t{};
			detail::allocate_1d_buf_if_vector(transposed_buf, cols, rows, Value{ 0 });

			for (auto col = std::size_t{ 0 }; col < cols; ++col)
			{
				for (auto row = std::size_t{ 0 }; row < rows; ++row)
				{
					auto normal_idx     = detail::idx_2d_to_1d(cols, row, col);
					auto transposed_idx = detail::idx_2d_to_1d(rows, col, row);

					transposed_buf[transposed_idx] = data[normal_idx];
				}
			}

			init_matrix_with_1d_rng_move(transposed, std::move(transposed_buf), cols, rows);

			return transposed;
		}

		template<typename... Args>
		[[nodiscard]] auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<transpose_t, Args...> // @TODO: ISSUE #20
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto transpose = transpose_t{};
} // namespace mpp