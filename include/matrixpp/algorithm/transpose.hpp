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

#include "../detail/matrix_base.hpp"
#include "../detail/utility.hpp"
#include "../matrix.hpp"

#include <cstddef>
#include <type_traits>

namespace matrixpp
{
	struct transpose_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(transpose_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<Value, ColumnsExtent, RowsExtent> // @TODO: ISSUE #20
		{
			auto transposed = matrix<Value, ColumnsExtent, RowsExtent>{};
			auto buf_copy   = obj.buffer();
			auto rows       = obj.rows();
			auto cols       = obj.columns();
			const auto& buf = obj.buffer();

			for (auto col = std::size_t{ 0 }; col < cols; ++col)
			{
				for (auto row = std::size_t{ 0 }; row < rows; ++row)
				{
					auto normal_idx     = detail::idx_2d_to_1d(cols, row, col);
					auto transposed_idx = detail::idx_2d_to_1d(rows, col, row);

					buf_copy[transposed_idx] = buf[normal_idx];
				}
			}

			detail::init_matrix_with_1d_rng_copy(transposed, std::move(buf_copy), cols, rows);

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
} // namespace matrixpp