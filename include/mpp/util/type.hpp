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
#include <mpp/mat/matfwd.hpp>

#include <cstddef>

namespace mpp
{
	enum class mat_type
	{
		fixed,
		dynamic,
		dynamic_rows,
		dynamic_cols
	};

	struct type_t : public detail::cpo_base<type_t>
	{
		template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
		[[nodiscard]] friend inline auto tag_invoke(type_t, const mat<Val, Rows, Cols, Alloc>&) noexcept
			-> mat_type // @TODO: ISSUE #20
		{
			constexpr auto row_is_dyn = Rows == dyn;
			constexpr auto col_is_dyn = Cols == dyn;

			if constexpr (!row_is_dyn && !col_is_dyn)
			{
				return mat_type::fixed;
			}
			else if constexpr (row_is_dyn && col_is_dyn)
			{
				return mat_type::dynamic;
			}
			else if constexpr (row_is_dyn && !col_is_dyn)
			{
				return mat_type::dynamic_rows;
			}
			else
			{
				return mat_type::dynamic_cols;
			}
		}
	};

	inline constexpr auto type = type_t{};
} // namespace mpp
