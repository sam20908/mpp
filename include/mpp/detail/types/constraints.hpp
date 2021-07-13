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
#include <concepts>
#include <iterator>
#include <ranges>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
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
	} // namespace detail

	template<detail::arithmetic, std::size_t, std::size_t, typename>
	class matrix;

	namespace detail
	{
		template<typename Iterator, typename Value>
		concept input_iterator_2d_with_value = requires(Iterator it)
		{
			requires std::input_iterator<Iterator>;
			requires std::ranges::range<Iterator>;
			requires std::same_as<std::iter_value_t<Iterator>, Value>;
		};

		template<typename Callable, typename Return, typename... Args>
		concept invocable_with_return_type =
			std::invocable<Callable, Args...> && std::same_as<std::invoke_result_t<Callable, Args...>, Return>;

		template<typename Range>
		using range_2d_value_t = std::ranges::range_value_t<std::ranges::range_value_t<Range>>;

		template<typename Range, typename Value>
		concept range_2d_with_value_type_convertible_to = std::convertible_to<range_2d_value_t<Range>, Value>;

		template<typename Range, typename Value>
		concept range_1d_with_value_type_convertible_to = std::convertible_to<std::ranges::range_value_t<Range>, Value>;

		// matrix_with_value_convertible_to

		template<typename>
		struct is_matrix : std::false_type
		{
		};

		template<arithmetic Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
		struct is_matrix<matrix<Value, RowsExtent, ColumnsExtent, Allocator>> : std::true_type
		{
		};

		template<typename T, typename To>
		concept matrix_with_value_convertible_to = is_matrix<std::remove_cvref_t<T>>::value &&
			std::convertible_to<typename std::remove_cvref_t<T>::value_type, To>;
	} // namespace detail
} // namespace mpp
