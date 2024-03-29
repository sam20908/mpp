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

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace mpp
{
	template<typename>
	inline constexpr auto is_dyn_buf = false;
	template<typename>
	inline constexpr auto is_fixed_buf = false;

	template<typename T, typename Alloc>
	inline constexpr auto is_dyn_buf<std::vector<T, Alloc>> = true;

	template<typename T, std::size_t N>
	inline constexpr auto is_fixed_buf<std::array<T, N>> = true;

	struct identity_tag
	{
	};

	inline constexpr auto identity = identity_tag{};
} // namespace mpp
