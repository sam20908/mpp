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

#include <mpp/matrix.hpp>

#include "../thirdparty/ut.hpp"

#include <compare>
#include <cstddef>
#include <type_traits>

using namespace boost::ut;

void compare_matrix_to_range_2d(const auto& matrix, const auto& range_2d, std::size_t rows, std::size_t columns)
{
	expect(matrix.rows() == rows);
	expect(matrix.columns() == columns);

	using matrix_value_t   = typename std::remove_cvref_t<decltype(matrix)>::value_type;
	using range_2d_value_t = typename std::remove_cvref_t<decltype(range_2d)>::value_type::value_type;
	using ordering_type    = std::compare_three_way_result_t<matrix_value_t, range_2d_value_t>;

	auto all_elems_equal = true;

	for (auto row = std::size_t{}; row < rows; ++row)
	{
		for (auto column = std::size_t{}; column < columns; ++column)
		{
			if (mpp::floating_point_compare(matrix(row, column), range_2d[row][column]) != ordering_type::equivalent)
			{
				all_elems_equal = false;
				break;
			}
		}
	}

	expect(all_elems_equal);
}

void compare_matrix_to_matrix(const auto& left_matrix, const auto& right_matrix)
{
	const auto [row_ordering, column_ordering] = mpp::size_compare(left_matrix, right_matrix, true, true);

	expect(row_ordering == std::partial_ordering::equivalent);
	expect(column_ordering == std::partial_ordering::equivalent);

	using ordering_type =
		std::compare_three_way_result_t<typename std::remove_cvref_t<decltype(left_matrix)>::value_type,
			typename std::remove_cvref_t<decltype(right_matrix)>::value_type>;

	expect(mpp::elements_compare(left_matrix, right_matrix, mpp::floating_point_compare) == ordering_type::equivalent);
}

inline constexpr auto compose = [](auto&& compose_fn, auto&&... args) -> decltype(auto) {
	return [compose_fn = compose_fn, ... args = args]() -> decltype(auto) {
		return std::invoke(FWD(compose_fn), FWD(args)...);
	};
};