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
#include <mpp/matrix.hpp>

#include <cstddef>

namespace mpp
{
	enum class matrix_type
	{
		fully_static,
		fully_dynamic,
		dynamic_rows,
		dynamic_columns
	};

	struct type_t : public detail::cpo_base<type_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
		[[nodiscard]] friend inline auto tag_invoke(type_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj) noexcept -> matrix_type // @TODO: ISSUE #20
		{
			constexpr auto row_is_dynamic    = obj.rows_extent() == dynamic;
			constexpr auto column_is_dynamic = obj.columns_extent() == dynamic;

			if constexpr (!row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::fully_static;
			}
			else if constexpr (row_is_dynamic && column_is_dynamic)
			{
				return matrix_type::fully_dynamic;
			}
			else if constexpr (row_is_dynamic && !column_is_dynamic)
			{
				return matrix_type::dynamic_rows;
			}
			else
			{
				return matrix_type::dynamic_columns;
			}
		}
	};

	inline constexpr auto type = type_t{};
} // namespace mpp
