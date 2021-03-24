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

#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include "../../thirdparty/ut.hpp"

#include <array>
#include <compare>
#include <span>
#include <vector>

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

void test_initialization(const auto& left_matrix_creator, const auto& right_matrix_creator)
{
	const auto left_matrix  = left_matrix_creator();
	const auto right_matrix = right_matrix_creator();

	const auto [row_ordering, column_ordering] = mpp::size_compare(left_matrix, right_matrix, true, true);

	expect(row_ordering == std::partial_ordering::equivalent);
	expect(column_ordering == std::partial_ordering::equivalent);

	using ordering_type =
		std::compare_three_way_result_t<typename std::remove_cvref_t<decltype(left_matrix)>::value_type,
			typename std::remove_cvref_t<decltype(right_matrix)>::value_type>;

	expect(mpp::elements_compare(left_matrix, right_matrix, mpp::floating_point_compare) == ordering_type::equivalent);
}

int main()
{
	// @NOTE: Most of the tests are referring to this 2d range
	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

	"Basic semantics"_test = [&]() {
		scenario("Creating matrices with 2D range") = [&]() {
			given("A 2D range") = [&]() {
				test_initialization(
					[&]() {
						return mpp::matrix<float, 2, 3>{ range_2d };
					},
					[&]() {
						return mpp::matrix{ range_2d };
					});
				test_initialization(
					[&]() {
						return mpp::matrix<double>{ range_2d };
					},
					[&]() {
						return mpp::matrix<int, 2, std::dynamic_extent>{ range_2d };
					});
			};
		};

		scenario("Assigning matrices with 2D range") = [&]() {
			const auto left_range_2d = std::array<std::array<int, 4>, 4>{
				{ { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } }
			};

			auto left_matrix  = mpp::matrix<int>{ left_range_2d };
			auto right_matrix = mpp::matrix<int, 2, 3>{ range_2d };

			left_matrix  = range_2d;
			right_matrix = range_2d;

			expect(mpp::elements_compare(left_matrix, right_matrix) == std::strong_ordering::equivalent);

			const auto big_range_2d = std::array<std::array<int, 10>, 10>{ { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 11, 22, 33, 44, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
				{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } } };

			auto big_left_matrix = mpp::matrix{ left_matrix };
			big_left_matrix      = big_range_2d;

			const auto big_right_matrix = mpp::matrix{ big_range_2d };

			expect(mpp::elements_compare(big_left_matrix, big_right_matrix) == std::strong_ordering::equivalent);
		};

		scenario("Matrices rule-of-five") = [&]() {
			auto left_matrix                   = mpp::matrix<int>{ range_2d };
			const auto matrix_copy_constructed = mpp::matrix<int, 2, 3>{ left_matrix };

			expect(mpp::elements_compare(left_matrix, matrix_copy_constructed) == std::strong_ordering::equivalent);

			const auto matrix_move_constructed = mpp::matrix<int, 2, 3>{ std::move(left_matrix) };
			expect(mpp::elements_compare(matrix_copy_constructed, matrix_move_constructed) ==
				   std::strong_ordering::equivalent);

			auto left_matrix_2        = mpp::matrix<int>{ range_2d };
			auto matrix_copy_assigned = mpp::matrix<int>{};
			matrix_copy_assigned      = left_matrix_2;

			expect(mpp::elements_compare(left_matrix_2, matrix_copy_assigned) == std::strong_ordering::equivalent);

			auto left_matrix_3 = mpp::matrix<float, 2, 3>{};
			left_matrix_3      = left_matrix_2;

			expect(mpp::elements_compare(left_matrix_2, left_matrix_3) == std::partial_ordering::equivalent);
		};
	};
}