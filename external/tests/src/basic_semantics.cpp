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

#include "../../include/utility.hpp"
#include "../../thirdparty/ut.hpp"

#include <array>
#include <compare>
#include <cstddef>
#include <vector>

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests

	// @NOTE: Most of the tests are referring to this 2d range
	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

	"Basic semantics"_test = [&]() {
		scenario("Creating matrices") = [&]() {
			given("A 2D range") = [&]() {
				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>(auto... additional_args) {
					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ range_2d, additional_args... };
					const auto matrix_2 =
						mpp::matrix<Value, mpp::dynamic, mpp::dynamic>{ range_2d, additional_args... };
					const auto matrix_3 = mpp::matrix<Value, Rows, mpp::dynamic>{ range_2d, additional_args... };
					const auto matrix_4 = mpp::matrix<Value, mpp::dynamic, Columns>{ range_2d, additional_args... };

					compare_matrix_to_range_2d(matrix_1, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_2, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_3, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_4, range_2d, Rows, Columns);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<float, 2, 3>(); // Test value convertibility
				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
				test_fn.template operator()<float, 2, 3>(mpp::unsafe); // Test value convertibility
			};

			given("A callable") = []() {
				const auto iota_range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>() {
					auto iota = []() {
						return [i = 1]() mutable {
							return i++;
						};
					};

					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ iota() };
					const auto matrix_2 = mpp::matrix<Value, mpp::dynamic, mpp::dynamic>{ Rows, Columns, iota() };
					const auto matrix_3 = mpp::matrix<Value, Rows, mpp::dynamic>{ Columns, iota() };
					const auto matrix_4 = mpp::matrix<Value, mpp::dynamic, Columns>{ Rows, iota() };

					compare_matrix_to_range_2d(matrix_1, iota_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_2, iota_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_3, iota_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_4, iota_range_2d, Rows, Columns);
				};

				test_fn.template operator()<int, 2, 3>();
				// test_fn.template operator()<float, 2, 3>(); // @FIXME: Allow callable's value return be convertible
				// to value typeest value convertibility
			};

			given("A 1D range") = [&]() {
				const auto range_1d = std::vector<int>{ 1, 2, 3, 4, 5, 6 };

				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>(auto... additional_args) {
					const auto matrix_1 =
						mpp::matrix<Value, Rows, Columns>{ Rows, Columns, range_1d, additional_args... };
					const auto matrix_2 =
						mpp::matrix<Value, mpp::dynamic, mpp::dynamic>{ Rows, Columns, range_1d, additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value, Rows, mpp::dynamic>{ Rows, Columns, range_1d, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value, mpp::dynamic, Columns>{ Rows, Columns, range_1d, additional_args... };

					compare_matrix_to_range_2d(matrix_1, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_2, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_3, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_4, range_2d, Rows, Columns);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
			};

			given("A value") = [&]() {
				const auto value_range_2d = std::vector<std::vector<int>>{ { 2, 2, 2 }, { 2, 2, 2 } };

				const auto matrix_1 = mpp::matrix<int, 2, 3>{ 2 };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ 2, 3, 2 };
				const auto matrix_3 = mpp::matrix<int, mpp::dynamic, 3>{ 2, 2 };
				const auto matrix_4 = mpp::matrix<int, 2, mpp::dynamic>{ 3, 2 };

				compare_matrix_to_range_2d(matrix_1, value_range_2d, 2, 3);
				compare_matrix_to_range_2d(matrix_2, value_range_2d, 2, 3);
				compare_matrix_to_range_2d(matrix_3, value_range_2d, 2, 3);
				compare_matrix_to_range_2d(matrix_4, value_range_2d, 2, 3);
			};

			given("A 2D initializer_list") = [&]() {
				const auto initializer_list_2d =
					std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>(auto... additional_args) {
					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ initializer_list_2d, additional_args... };
					const auto matrix_2 =
						mpp::matrix<Value, mpp::dynamic, mpp::dynamic>{ initializer_list_2d, additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value, Rows, mpp::dynamic>{ initializer_list_2d, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value, mpp::dynamic, Columns>{ initializer_list_2d, additional_args... };

					compare_matrix_to_range_2d(matrix_1, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_2, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_3, range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_4, range_2d, Rows, Columns);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<float, 2, 3>(); // Test value convertibility
				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
				test_fn.template operator()<float, 2, 3>(mpp::unsafe); // Test value convertibility
			};

			given("A matrix of different value type or extents") = [&]() {
				{
					const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ range_2d };
					const auto matrix_3 = mpp::matrix<long, mpp::dynamic, 3>{ range_2d };
					const auto matrix_4 = mpp::matrix<short, 2, mpp::dynamic>{ range_2d };

					compare_matrix_to_range_2d(mpp::matrix<short, 2, 3>{ matrix_4 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ matrix_3 },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(mpp::matrix<int, mpp::dynamic, 3>{ matrix_2 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<double, 2, mpp::dynamic>{ matrix_1 }, range_2d, 2, 3);
				}

				{
					const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d, mpp::unsafe };
					const auto matrix_2 = mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ range_2d, mpp::unsafe };
					const auto matrix_3 = mpp::matrix<long, mpp::dynamic, 3>{ range_2d, mpp::unsafe };
					const auto matrix_4 = mpp::matrix<short, 2, mpp::dynamic>{ range_2d, mpp::unsafe };

					compare_matrix_to_range_2d(mpp::matrix<short, 2, 3>{ matrix_4 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ matrix_3 },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(mpp::matrix<int, mpp::dynamic, 3>{ matrix_2 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<double, 2, mpp::dynamic>{ matrix_1 }, range_2d, 2, 3);
				}
			};

			scenario("Constructing an identity matrix") = [&]() {
				const auto identity_range_2d = std::vector<std::vector<int>>{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>(auto... additional_args) {
					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ mpp::identity, additional_args... };
					const auto matrix_2 = mpp::matrix<Value, mpp::dynamic, mpp::dynamic>{ Rows,
						Columns,
						mpp::identity,
						additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value, Rows, mpp::dynamic>{ Columns, mpp::identity, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value, mpp::dynamic, Columns>{ Rows, mpp::identity, additional_args... };

					compare_matrix_to_range_2d(matrix_1, identity_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_2, identity_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_3, identity_range_2d, Rows, Columns);
					compare_matrix_to_range_2d(matrix_4, identity_range_2d, Rows, Columns);
				};

				test_fn.template operator()<int, 3, 3>();
				test_fn.template operator()<int, 3, 3>(mpp::unsafe);
			};
		};

		scenario("Comparing matrices") = [&]() {
			auto test_fn = [&]<typename LeftValue,
							   typename RightValue,
							   std::size_t LeftRows,
							   std::size_t LeftColumns,
							   std::size_t RightRows,
							   std::size_t RightColumns>() {
				const auto matrix_1   = mpp::matrix<LeftValue, LeftRows, LeftColumns>{ range_2d };
				const auto matrix_1_1 = mpp::matrix<RightValue, RightRows, RightColumns>{ range_2d };
				compare_matrix_to_matrix(matrix_1, matrix_1_1);

				const auto matrix_1_2 = mpp::matrix<RightValue, mpp::dynamic, mpp::dynamic>{ range_2d };
				compare_matrix_to_matrix(matrix_1, matrix_1_2);

				const auto matrix_1_3 = mpp::matrix<RightValue, RightRows, mpp::dynamic>{ range_2d };
				compare_matrix_to_matrix(matrix_1, matrix_1_3);

				const auto matrix_1_4 = mpp::matrix<RightValue, mpp::dynamic, RightColumns>{ range_2d };
				compare_matrix_to_matrix(matrix_1, matrix_1_4);
			};
			test_fn.template operator()<int, int, 2, 3, 2, 3>();
			test_fn.template operator()<double, short, 2, 3, 2, 3>(); // Test different value type comparison
		};

		scenario("Assigning matrices") = [&]() {
			scenario("Assigning from a 2D range") = [&]() {
				given("A bigger range") = [&]() {
					// Test insertion machinery (only for dynamic matrices)

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

					{
						auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
						auto matrix_2 = mpp::matrix<int, 10, mpp::dynamic>{ range_2d };
						auto matrix_3 = mpp::matrix<int, mpp::dynamic, 10>{ range_2d };

						matrix_1 = big_range_2d;
						matrix_2 = big_range_2d;
						matrix_3 = big_range_2d;

						compare_matrix_to_range_2d(matrix_1, big_range_2d, 10, 10);
						compare_matrix_to_range_2d(matrix_2, big_range_2d, 10, 10);
						compare_matrix_to_range_2d(matrix_3, big_range_2d, 10, 10);
					}
				};

				given("A smaller range") = [&]() {
					// Test resize machinery (only for dynamic matrices)

					const auto small_range_2d = std::array<std::array<int, 2>, 1>{ { { 1, 1 } } };

					auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
					auto matrix_2 = mpp::matrix<int, 1, mpp::dynamic>{ range_2d };
					auto matrix_3 = mpp::matrix<int, mpp::dynamic, 2>{ range_2d };

					matrix_1 = small_range_2d;
					matrix_2 = small_range_2d;
					matrix_3 = small_range_2d;

					compare_matrix_to_range_2d(matrix_1, small_range_2d, 1, 2);
					compare_matrix_to_range_2d(matrix_2, small_range_2d, 1, 2);
					compare_matrix_to_range_2d(matrix_3, small_range_2d, 1, 2);
				};

				given("A range that is the same size") = [&]() {
					const auto another_range_2d_v = std::vector<std::vector<int>>{ { { 4, 5, 6 }, { 7, 8, 9 } } };
					auto another_range_2d_v2      = std::vector<std::vector<int>>{ { { 4, 5, 6 }, { 7, 8, 9 } } };

					auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
					auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
					auto matrix_3 = mpp::matrix<int, 1, mpp::dynamic>{ range_2d };
					auto matrix_4 = mpp::matrix<int, mpp::dynamic, 2>{ range_2d };

					// Test const Range2D& overload

					matrix_1 = another_range_2d_v;
					matrix_2 = another_range_2d_v;
					matrix_3 = another_range_2d_v;
					matrix_4 = another_range_2d_v;

					compare_matrix_to_range_2d(matrix_1,
						another_range_2d_v,
						2,
						3); // Test constrained const Range2D& overload
					compare_matrix_to_range_2d(matrix_2, another_range_2d_v, 2, 3);
					compare_matrix_to_range_2d(matrix_3, another_range_2d_v, 2, 3);
					compare_matrix_to_range_2d(matrix_4, another_range_2d_v, 2, 3);

					// Test Range2D&& overload

					matrix_1 = std::move(another_range_2d_v2);
					matrix_2 = std::move(another_range_2d_v2);
					matrix_3 = std::move(another_range_2d_v2);
					matrix_4 = std::move(another_range_2d_v2);

					compare_matrix_to_range_2d(matrix_1,
						another_range_2d_v,
						2,
						3); // Test constrained Range2D&& overload
					compare_matrix_to_range_2d(matrix_2, another_range_2d_v, 2, 3);
					compare_matrix_to_range_2d(matrix_3, another_range_2d_v, 2, 3);
					compare_matrix_to_range_2d(matrix_4, another_range_2d_v, 2, 3);

					const auto another_range_2d_a =
						std::array<std::array<int, 3>, 2>{ { { 3, 2, 61 }, { 75, 86, 92 } } };
					auto another_range_2d_a2 = std::array<std::array<int, 3>, 2>{ { { 3, 2, 61 }, { 75, 86, 92 } } };

					// Test const std::array& overload

					matrix_1 = another_range_2d_a;

					compare_matrix_to_range_2d(matrix_1, another_range_2d_a, 2, 3);

					// Test std::array&& overload

					matrix_1 = std::move(another_range_2d_a2);

					compare_matrix_to_range_2d(matrix_1, another_range_2d_a, 2, 3);
				};

				given("A 2D initializer_list") = [&]() {
					const auto initializer_list_2d =
						std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

					auto matrix_1 = mpp::matrix<int, 2, 3>{};
					auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
					auto matrix_3 = mpp::matrix<int, 1, mpp::dynamic>{};
					auto matrix_4 = mpp::matrix<int, mpp::dynamic, 2>{};

					matrix_1 = initializer_list_2d;
					matrix_2 = initializer_list_2d;
					matrix_3 = initializer_list_2d;
					matrix_4 = initializer_list_2d;


					compare_matrix_to_range_2d(matrix_1, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_3, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_4, range_2d, 2, 3);
				};

				given("A matrix of different value type or extents") = [&]() {
					const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ range_2d };
					const auto matrix_3 = mpp::matrix<long, mpp::dynamic, 3>{ range_2d };
					const auto matrix_4 = mpp::matrix<short, 2, mpp::dynamic>{ range_2d };

					auto matrix_v1 = mpp::matrix<short, 2, 3>{};
					auto matrix_v2 = mpp::matrix<float, mpp::dynamic, mpp::dynamic>{};
					auto matrix_v3 = mpp::matrix<int, mpp::dynamic, 3>{};
					auto matrix_v4 = mpp::matrix<double, 2, mpp::dynamic>{};

					// Test constrained const Matrix& overload

					matrix_v1 = matrix_4;
					matrix_v2 = matrix_3;
					matrix_v3 = matrix_2;
					matrix_v4 = matrix_1;

					compare_matrix_to_range_2d(matrix_v1, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v2, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v3, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v4, range_2d, 2, 3);

					// Test constrained Matrix&& overload

					matrix_v1 = std::move(matrix_4);
					matrix_v2 = std::move(matrix_3);
					matrix_v3 = std::move(matrix_2);
					matrix_v4 = std::move(matrix_1);

					compare_matrix_to_range_2d(matrix_v1, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v2, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v3, range_2d, 2, 3);
					compare_matrix_to_range_2d(matrix_v4, range_2d, 2, 3);
				};
			};
		};

		scenario("Matrices rule-of-five") = [&]() {
			scenario("Copy constructing from another matrix") = [&]() {
				{
					const auto matrix   = mpp::matrix<int, 2, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, 2, 3>{ matrix };

					compare_matrix_to_matrix(matrix, matrix_2);
				}

				{
					const auto matrix   = mpp::matrix<int>{ range_2d };
					const auto matrix_2 = mpp::matrix<int>{ matrix };

					compare_matrix_to_matrix(matrix, matrix_2);
				}

				{
					const auto matrix   = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, mpp::dynamic, 3>{ matrix };

					compare_matrix_to_matrix(matrix, matrix_2);
				}

				{
					const auto matrix   = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic>{ matrix };

					compare_matrix_to_matrix(matrix, matrix_2);
				}
			};

			scenario("Move constructing from another matrix") = [&]() {
				{
					auto matrix         = mpp::matrix<int, 2, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, 2, 3>{ std::move(matrix) };

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}

				{
					auto matrix         = mpp::matrix<int>{ range_2d };
					const auto matrix_2 = mpp::matrix<int>{ std::move(matrix) };

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}

				{
					auto matrix         = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, mpp::dynamic, 3>{ std::move(matrix) };

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}

				{
					auto matrix         = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic>{ std::move(matrix) };

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}
			};

			scenario("Copy assigning from matrices") = [&]() {
				{
					const auto matrix = mpp::matrix<int, 2, 3>{ range_2d };
					auto matrix_2     = mpp::matrix<int, 2, 3>{};

					matrix_2 = matrix;

					compare_matrix_to_matrix(matrix, matrix_2);
				}

				{
					const auto matrix = mpp::matrix<int>{ range_2d };
					auto matrix_2     = mpp::matrix<int>{};

					matrix_2 = matrix;

					compare_matrix_to_matrix(matrix, matrix_2);
				}


				{
					const auto matrix = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
					auto matrix_2     = mpp::matrix<int, mpp::dynamic, 3>{};

					matrix_2 = matrix;

					compare_matrix_to_matrix(matrix, matrix_2);
				}

				{
					const auto matrix = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
					auto matrix_2     = mpp::matrix<int, 2, mpp::dynamic>{};

					matrix_2 = matrix;

					compare_matrix_to_matrix(matrix, matrix_2);
				}
			};

			scenario("Move assigning from matrices") = [&]() {
				{
					auto matrix   = mpp::matrix<int, 2, 3>{ range_2d };
					auto matrix_2 = mpp::matrix<int, 2, 3>{};

					matrix_2 = std::move(matrix);

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}

				{
					auto matrix   = mpp::matrix<int>{ range_2d };
					auto matrix_2 = mpp::matrix<int>{};

					matrix_2 = std::move(matrix);

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}


				{
					auto matrix   = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };
					auto matrix_2 = mpp::matrix<int, mpp::dynamic, 3>{};

					matrix_2 = std::move(matrix);

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}

				{
					auto matrix   = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
					auto matrix_2 = mpp::matrix<int, 2, mpp::dynamic>{};

					matrix_2 = std::move(matrix);

					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				}
			};

			scenario("Matrices should be destructible") = [&]() {
				expect(constant<std::is_destructible_v<mpp::matrix<int, 2, 3>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, 3>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, 2, mpp::dynamic>>>);
			};
		};
	};
}