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
#include <iterator>
#include <ranges>
#include <type_traits>

namespace mpp::detail
{
	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	class matrix_base;

	template<typename Derived,
		typename Buffer,
		typename Value,
		std::size_t RowsExtent,
		std::size_t ColumnsExtent,
		typename Allocator>
	constexpr auto matrix_like_resolver(
		const matrix_base<Derived, Buffer, Value, RowsExtent, ColumnsExtent, Allocator>&) -> int;

	constexpr void matrix_like_resolver(...);

	template<typename T>
	static constexpr auto matrix_like_v = std::is_same_v<decltype(matrix_like_resolver(std::declval<T>())), int>;

	template<typename T>
	concept matrix_like = matrix_like_v<T>;

	template<typename T, typename Value>
	concept matrix_like_with_value_type_convertible_to =
		matrix_like<T>&& std::convertible_to<typename std::remove_cvref_t<T>::value_type, Value>;

	template<typename Value>
	concept arithmetic = requires(Value&& value)
	{
		{ value + value };
		{ value - value };
		{ value * value };
		{ value / value };
		{ value += value };
		{ value -= value };
		{ value *= value };
		{ value /= value };
	};

	template<typename Iterator, typename Value>
	concept input_iterator_2d_with_value = requires(Iterator it)
	{
		requires std::input_iterator<Iterator>;
		requires std::ranges::range<Iterator>;
		requires std::same_as<std::iter_value_t<Iterator>, Value>;
	};

	template<typename Callable, typename Return, typename... Args>
	concept invocable_with_return_type =
		std::invocable<Callable, Args...>&& std::same_as<std::invoke_result_t<Callable, Args...>, Return>;

	template<typename Range>
	using range_2d_value_t = std::ranges::range_value_t<std::ranges::range_value_t<Range>>;

	template<typename Range, typename Value>
	concept range_2d_with_value_type_convertible_to = std::convertible_to<range_2d_value_t<Range>, Value>;

	template<typename Range, typename Value>
	concept range_1d_with_value_type_convertible_to = std::convertible_to<std::ranges::range_value_t<Range>, Value>;
} // namespace mpp::detail