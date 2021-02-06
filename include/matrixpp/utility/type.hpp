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

#include <matrixpp/detail/tag_invoke.hpp>
#include <matrixpp/matrix.hpp>

#include <cstddef>
#include <span>

namespace matrixpp
{
	enum class matrix_type
	{
		fully_static,
		fully_dynamic,
		dynamic_rows,
		dynamic_columns
	};

	struct type_t
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto tag_invoke(type_t, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix_type // @TODO: ISSUE #20
		{
			auto row_is_dynamic    = obj.rows_extent() == std::dynamic_extent;
			auto column_is_dynamic = obj.columns_extent() == std::dynamic_extent;

			if (!row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::fully_static;
			}
			else if (row_is_dynamic && column_is_dynamic)
			{
				return matrix_type::fully_dynamic;
			}
			else if (row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::dynamic_rows;
			}
			else
			{
				return matrix_type::dynamic_columns;
			}
		}

		template<typename... Args>
		[[nodiscard]] auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<type_t, Args...> // @TODO: ISSUE #20
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto type = type_t{};
} // namespace matrixpp