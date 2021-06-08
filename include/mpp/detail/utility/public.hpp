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
#include <memory>

// @TODO: Export this header to the user for modules

namespace mpp
{
	static inline constexpr auto dynamic = static_cast<std::size_t>(-1);

	template<std::size_t Val = dynamic>
	struct constant
	{
		std::size_t val{ Val };

		constexpr explicit operator std::size_t() const
		{
			return val;
		}

		[[nodiscard]] static constexpr auto tmp_val() noexcept -> std::size_t
		{
			return Val;
		}
	};

	/**
	 * Tags
	 */

	struct identity_tag
	{
	};

	struct unsafe_tag
	{
	};

	inline constexpr auto identity = identity_tag{};
	inline constexpr auto unsafe   = unsafe_tag{};
} // namespace mpp
