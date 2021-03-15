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

#include <cstddef>
#include <span>

namespace mpp::customize_extents
{
	struct rows_extent_tag
	{
	};

	struct columns_extent_tag
	{
	};

	struct customize_tag
	{
	};

	inline constexpr auto customize      = customize_tag{};
	inline constexpr auto rows_extent    = rows_extent_tag{};
	inline constexpr auto columns_extent = columns_extent_tag{};

	template<typename... Args>
	[[nodiscard]] constexpr auto tag_invoke(rows_extent_tag, Args&&...) -> std::size_t
	{
		return std::dynamic_extent;
	}

	template<typename... Args>
	[[nodiscard]] constexpr auto tag_invoke(columns_extent_tag, Args&&...) -> std::size_t
	{
		return std::dynamic_extent;
	}
} // namespace mpp::customize_extents