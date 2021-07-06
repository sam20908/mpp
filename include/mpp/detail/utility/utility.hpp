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

#include <mpp/detail/types/constraints.hpp>
#include <mpp/detail/utility/public.hpp>
#include <mpp/utility/configuration.hpp>

#include <cstddef>

namespace mpp::detail
{
	[[nodiscard]] constexpr auto
	index_2d_to_1d(std::size_t columns, std::size_t row_index, std::size_t column_index) noexcept -> std::size_t
	{
		// This is mainly for avoiding bug-prone code, because this calculation occurs in a lot of places, and a typo
		// can cause a lot of things to fail. It's safer to wrap this calculation in a function, so the bug is easier to
		// spot. This also assumes that the storage of row-major

		return row_index * columns + column_index;
	}

	template<typename... Args>
	[[nodiscard]] constexpr auto any_extent_is_dynamic(Args... args) noexcept -> std::size_t
	{
		return ((args == dynamic) || ...);
	}

	template<typename T>
	[[nodiscard]] constexpr auto get_constant_val_or_dynamic() noexcept -> std::size_t
	{
		if constexpr (is_constant<T>::value)
		{
			return T::tmp_val();
		}
		else
		{
			return dynamic;
		}
	}
} // namespace mpp::detail
