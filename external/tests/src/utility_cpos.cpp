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

// @TODO: Move cast to algorithms when #162 is being worked on

#include <mpp/matrix.hpp>
#include <mpp/utility.hpp>

#include "../../include/utility.hpp"
#include "../../thirdparty/ut.hpp"

#include <compare>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_type(mpp::matrix_type type)
{
	const auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{};

	expect(mpp::type(matrix) == type);
}

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_square(auto... dimension_args)
{
	const auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ dimension_args... };

	expect(mpp::square(matrix));
}

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_singular(auto val, bool is_singular, auto... dimension_args)
{
	const auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ dimension_args..., val };

	expect(mpp::singular(matrix) == is_singular);
}

template<typename To, std::size_t RowsExtent, std::size_t ColumnsExtent>
void test_cast(const std::vector<std::vector<int>>& rng_2d)
{
	const auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ rng_2d };
	const auto casted = mpp::cast(std::type_identity<To>{}, matrix);

	expect(matrix.rows() == casted.rows());
	expect(matrix.columns() == casted.columns());
	expect(matrix.rows_extent() == casted.rows_extent());
	expect(matrix.columns_extent() == casted.columns_extent());

	using ordered_type = std::compare_three_way_result_t<To, To>;

	expect(mpp::elements_compare(matrix, casted, mpp::floating_point_compare) == ordered_type::equivalent);
}

void test_size_compare(const auto& left_matrix_creator,
	const auto& right_matrix_creator,
	std::partial_ordering row_ordering,
	std::partial_ordering column_ordering,
	bool equal_in_rows,
	bool equal_in_columns)
{
	const auto left  = left_matrix_creator();
	const auto right = right_matrix_creator();
	const auto [compare_row_ordering, compare_column_ordering] =
		mpp::size_compare(left, right, equal_in_rows, equal_in_columns);

	expect(row_ordering == compare_row_ordering);
	expect(column_ordering == compare_column_ordering);
}

template<typename CompareThreeWay = std::compare_three_way>
void test_elements_compare(auto&& left_matrix_creator,
	auto&& right_matrix_creator,
	auto ordering,
	CompareThreeWay compare_three_way_fn = {})
{
	const auto left             = left_matrix_creator();
	const auto right            = right_matrix_creator();
	const auto compare_ordering = mpp::elements_compare(left, right, compare_three_way_fn);

	expect(compare_ordering == ordering);
}

int main()
{
	feature("utility CPOs") = []() {
		scenario("using type CPO") = []() {
			test_type<1, 1>(mpp::matrix_type::fully_static);
			test_type<std::dynamic_extent, std::dynamic_extent>(mpp::matrix_type::fully_dynamic);
			test_type<std::dynamic_extent, 1>(mpp::matrix_type::dynamic_rows);
			test_type<1, std::dynamic_extent>(mpp::matrix_type::dynamic_columns);
		};

		scenario("using square CPO on square matrices") = []() {
			test_square<1, 1>();
			test_square<std::dynamic_extent, std::dynamic_extent>(1ul, 1ul);
			test_square<std::dynamic_extent, 1>(1ul);
			test_square<1, std::dynamic_extent>(1ul);
		};

		scenario("using singular CPO") = []() {
			// Most common scenario for singular matrices is all elements are 0

			when("checking against non-singular matrices") = []() {
				test_singular<1, 1>(1, false);
				test_singular<std::dynamic_extent, std::dynamic_extent>(1, false, 1ul, 1ul);
				test_singular<std::dynamic_extent, 1>(1, false, 1ul);
				test_singular<1, std::dynamic_extent>(1, false, 1ul);
			};

			when("checking against singular matrices") = []() {
				test_singular<1, 1>(0, true);
				test_singular<std::dynamic_extent, std::dynamic_extent>(0, true, 1ul, 1ul);
				test_singular<std::dynamic_extent, 1>(0, true, 1ul);
				test_singular<1, std::dynamic_extent>(0, true, 1ul);
			};
		};

		scenario("using cast CPO") = []() {
			auto rng_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

			test_cast<float, 2, 3>(rng_2d);
			test_cast<float, std::dynamic_extent, std::dynamic_extent>(rng_2d);
			test_cast<float, std::dynamic_extent, 3>(rng_2d);
			test_cast<float, 2, std::dynamic_extent>(rng_2d);
		};

		scenario("comparison CPOs") = []() {
			when("using size_compare CPO") = []() {
				test_size_compare(
					[]() {
						return mpp::matrix<int, 1, 1>{};
					},
					[]() {
						return mpp::matrix<int, 1, 1>{};
					},
					std::partial_ordering::equivalent,
					std::partial_ordering::equivalent,
					true,
					true);
				test_size_compare(
					[]() {
						return mpp::matrix<int, 1, 1>{};
					},
					[]() {
						return mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{ 1, 1 };
					},
					std::partial_ordering::unordered,
					std::partial_ordering::unordered,
					false,
					false);
				test_size_compare(
					[]() {
						return mpp::matrix<int, 1, 1>{};
					},
					[]() {
						return mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{ 1, 3 };
					},
					std::partial_ordering::unordered,
					std::partial_ordering::less,
					false,
					true);
				test_size_compare(
					[]() {
						return mpp::matrix<int, 2, 1>{};
					},
					[]() {
						return mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{ 1, 1 };
					},
					std::partial_ordering::greater,
					std::partial_ordering::unordered,
					true,
					false);
			};

			when("using elements_cmopare CPO") = []() {
				test_elements_compare(
					[]() {
						return mpp::matrix<int, 1, 1>{ 0 };
					},
					[]() {
						return mpp::matrix<int, 1, 1>{ 1 };
					},
					std::strong_ordering::less);
				test_elements_compare(
					[]() {
						return mpp::matrix<int, 2, 3>{ { 1, 2, 3 }, { 5, 6, 8 } };
					},
					[]() {
						return mpp::matrix{ { 1, 2, 3 }, { 5, 6, 7 } };
					},
					std::strong_ordering::greater);
				test_elements_compare(
					[]() {
						return mpp::matrix<int>{};
					},
					[]() {
						return mpp::matrix<int>{};
					},
					std::strong_ordering::equivalent);
				test_elements_compare(
					[]() {
						return mpp::matrix<float>{ 1, 1, 5.F / 3.F };
					},
					[]() {
						return mpp::matrix<float>{ 1, 1, 1.F };
					},
					std::partial_ordering::greater,
					mpp::floating_point_compare);
			};
		};
	};

	return 0;
}
