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

#include <concepts>
#include <ranges>
#include <type_traits>

namespace mpp::detail
{
	template<typename Value>
	concept arithmetic = requires(Value&& value)
	{
		// +, -, *, and /
		{ value + value };
		{ value - value };
		{ value * value };
		{ value / value };

		// +=, -=, *=, and /=
		{ value += value };
		{ value -= value };
		{ value *= value };
		{ value /= value };
	};

	template<typename Range>
	using range_2d_t = std::ranges::range_value_t<std::ranges::range_value_t<Range>>;

	template<typename Range, typename Value>
	concept range_2d_with_type = std::same_as<range_2d_t<Range>, Value>;
} // namespace mpp::detail