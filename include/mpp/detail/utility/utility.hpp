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

	[[nodiscard]] constexpr auto any_extent_is_dynamic(std::size_t row_extent, std::size_t column_extent) noexcept
		-> std::size_t
	{
		return row_extent == dynamic || column_extent == dynamic;
	}

	static constexpr auto configuration_use_unsafe = configuration<override>::use_unsafe;
	static constexpr auto configuration_use_safe   = !configuration_use_unsafe;
} // namespace mpp::detail
