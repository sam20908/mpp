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

#include "../../include/custom_allocator.hpp"
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

	// Next free template parameter suffix is 10

	// @NOTE: Most of the tests are referring to this 2d range
	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

	"Basic semantics"_test = [&]() {
		scenario("Creating matrices") = [&]() {
			scenario("Default initialization") = []() {
				const auto empty_range_2d = std::vector<std::vector<int>>{};

				const auto matrix_1 = mpp::matrix<int, 0, 0>{};
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
				const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic>{};
				const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 0>{};

				expect(matrix_1.rows() == 0_ul);
				expect(matrix_1.columns() == 0_ul);
				compare_matrix_to_range_2d(matrix_1, empty_range_2d, std::size_t{}, std::size_t{});

				expect(matrix_2.rows() == 0_ul);
				expect(matrix_2.columns() == 0_ul);
				compare_matrix_to_range_2d(matrix_2, empty_range_2d, std::size_t{}, std::size_t{});

				expect(matrix_3.rows() == 0_ul);
				expect(matrix_3.columns() == 0_ul);
				compare_matrix_to_range_2d(matrix_3, empty_range_2d, std::size_t{}, std::size_t{});

				expect(matrix_4.rows() == 0_ul);
				expect(matrix_4.columns() == 0_ul);
				compare_matrix_to_range_2d(matrix_4, empty_range_2d, std::size_t{}, std::size_t{});
			};

			given("Default initialize with custom allocators in dynamic matrices") = []() {
				const auto allocator      = custom_allocator<int>{};
				const auto empty_range_2d = std::vector<std::vector<int>>{};

				const auto matrix_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ allocator };
				const auto matrix_2 = mpp::matrix<int, mpp::dynamic, 0, custom_allocator<int>>{ allocator };
				const auto matrix_3 = mpp::matrix<int, 0, mpp::dynamic, custom_allocator<int>>{ allocator };

				compare_matrix_to_range_2d(matrix_1, empty_range_2d, 0, 0);
				compare_matrix_to_range_2d(matrix_2, empty_range_2d, 0, 0);
				compare_matrix_to_range_2d(matrix_3, empty_range_2d, 0, 0);
			};

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

			given("A std::array for fully static matrices") = [&]() {
				auto test_fn = [&]<typename Value9>() {
					const auto range_2d_array = std::array<std::array<int, 3>, 2>{ { { 1, 2, 3 }, { 4, 5, 6 } } };

					const auto matrix_1 = mpp::matrix<Value9, 2, 3>{ range_2d_array };
					compare_matrix_to_range_2d(matrix_1, range_2d, 2, 3);

					const auto matrix_2 =
						mpp::matrix<Value9, 2, 3>{ std::array<std::array<int, 3>, 2>{ { { 1, 2, 3 }, { 4, 5, 6 } } } };
					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				};

				test_fn.template operator()<int>();
				test_fn.template operator()<float>(); // Test value convertibility
			};

			given("A callable") = []() {
				const auto iota_range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

				auto test_fn = [&]<typename Value2, std::size_t Rows2, std::size_t Columns2>() {
					auto iota = []() {
						return [i = 1]() mutable {
							return i++;
						};
					};

					const auto matrix_1 = mpp::matrix<Value2, Rows2, Columns2>{ iota() };
					const auto matrix_2 = mpp::matrix<Value2, mpp::dynamic, mpp::dynamic>{ Rows2, Columns2, iota() };
					const auto matrix_3 = mpp::matrix<Value2, Rows2, mpp::dynamic>{ Columns2, iota() };
					const auto matrix_4 = mpp::matrix<Value2, mpp::dynamic, Columns2>{ Rows2, iota() };

					compare_matrix_to_range_2d(matrix_1, iota_range_2d, Rows2, Columns2);
					compare_matrix_to_range_2d(matrix_2, iota_range_2d, Rows2, Columns2);
					compare_matrix_to_range_2d(matrix_3, iota_range_2d, Rows2, Columns2);
					compare_matrix_to_range_2d(matrix_4, iota_range_2d, Rows2, Columns2);
				};

				test_fn.template operator()<int, 2, 3>();
				// test_fn.template operator()<float, 2, 3>(); // @FIXME: Allow callable's value return be convertible
				// to value type
			};

			given("A 1D range") = [&]() {
				const auto range_1d = std::vector<int>{ 1, 2, 3, 4, 5, 6 };

				auto test_fn = [&]<typename Value3, std::size_t Rows3, std::size_t Columns3>(auto... additional_args) {
					const auto matrix_1 =
						mpp::matrix<Value3, Rows3, Columns3>{ Rows3, Columns3, range_1d, additional_args... };
					const auto matrix_2 = mpp::matrix<Value3, mpp::dynamic, mpp::dynamic>{ Rows3,
						Columns3,
						range_1d,
						additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value3, Rows3, mpp::dynamic>{ Rows3, Columns3, range_1d, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value3, mpp::dynamic, Columns3>{ Rows3, Columns3, range_1d, additional_args... };

					compare_matrix_to_range_2d(matrix_1, range_2d, Rows3, Columns3);
					compare_matrix_to_range_2d(matrix_2, range_2d, Rows3, Columns3);
					compare_matrix_to_range_2d(matrix_3, range_2d, Rows3, Columns3);
					compare_matrix_to_range_2d(matrix_4, range_2d, Rows3, Columns3);
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

				auto test_fn = [&]<typename Value4, std::size_t Rows4, std::size_t Columns4>(auto... additional_args) {
					const auto matrix_1 =
						mpp::matrix<Value4, Rows4, Columns4>{ initializer_list_2d, additional_args... };
					const auto matrix_2 =
						mpp::matrix<Value4, mpp::dynamic, mpp::dynamic>{ initializer_list_2d, additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value4, Rows4, mpp::dynamic>{ initializer_list_2d, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value4, mpp::dynamic, Columns4>{ initializer_list_2d, additional_args... };

					compare_matrix_to_range_2d(matrix_1, range_2d, Rows4, Columns4);
					compare_matrix_to_range_2d(matrix_2, range_2d, Rows4, Columns4);
					compare_matrix_to_range_2d(matrix_3, range_2d, Rows4, Columns4);
					compare_matrix_to_range_2d(matrix_4, range_2d, Rows4, Columns4);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<float, 2, 3>(); // Test value convertibility
				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
				test_fn.template operator()<float, 2, 3>(mpp::unsafe); // Test value convertibility
			};

			given("A matrix of different value type or extents") = [&]() {
				{
					auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
					auto matrix_2 = mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ range_2d };
					auto matrix_3 = mpp::matrix<long, mpp::dynamic, 3>{ range_2d };
					auto matrix_4 = mpp::matrix<short, 2, mpp::dynamic>{ range_2d };

					compare_matrix_to_range_2d(mpp::matrix<short, 2, 3>{ matrix_4 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ matrix_3 },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(mpp::matrix<int, mpp::dynamic, 3>{ matrix_2 }, range_2d, 2, 3);
					compare_matrix_to_range_2d(mpp::matrix<double, 2, mpp::dynamic>{ matrix_1 }, range_2d, 2, 3);

					compare_matrix_to_range_2d(mpp::matrix<short, 2, 3>{ std::move(matrix_4), mpp::unsafe },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(
						mpp::matrix<float, mpp::dynamic, mpp::dynamic>{ std::move(matrix_3), mpp::unsafe },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(mpp::matrix<int, mpp::dynamic, 3>{ std::move(matrix_2), mpp::unsafe },
						range_2d,
						2,
						3);
					compare_matrix_to_range_2d(mpp::matrix<double, 2, mpp::dynamic>{ std::move(matrix_1), mpp::unsafe },
						range_2d,
						2,
						3);
				}
			};

			scenario("Constructing an identity matrix") = [&]() {
				const auto identity_range_2d = std::vector<std::vector<int>>{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

				auto test_fn = [&]<typename Value5, std::size_t Rows5, std::size_t Columns5>(auto... additional_args) {
					const auto matrix_1 = mpp::matrix<Value5, Rows5, Columns5>{ mpp::identity, additional_args... };
					const auto matrix_2 = mpp::matrix<Value5, mpp::dynamic, mpp::dynamic>{ Rows5,
						Columns5,
						mpp::identity,
						additional_args... };
					const auto matrix_3 =
						mpp::matrix<Value5, Rows5, mpp::dynamic>{ Columns5, mpp::identity, additional_args... };
					const auto matrix_4 =
						mpp::matrix<Value5, mpp::dynamic, Columns5>{ Rows5, mpp::identity, additional_args... };

					compare_matrix_to_range_2d(matrix_1, identity_range_2d, Rows5, Columns5);
					compare_matrix_to_range_2d(matrix_2, identity_range_2d, Rows5, Columns5);
					compare_matrix_to_range_2d(matrix_3, identity_range_2d, Rows5, Columns5);
					compare_matrix_to_range_2d(matrix_4, identity_range_2d, Rows5, Columns5);
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

				given("A std::array for fully static matrices") = [&]() {
					auto range_2d_array = std::array<std::array<int, 3>, 2>{ { { 1, 2, 3 }, { 4, 5, 6 } } };
					auto matrix_1       = mpp::matrix<int, 2, 3>{};
					matrix_1.assign(range_2d_array);
					compare_matrix_to_range_2d(matrix_1, range_2d, 2, 3);

					auto matrix_2 = mpp::matrix<int, 2, 3>{};
					matrix_2.assign(std::move(range_2d_array));
					compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
				};

				given("A range that is the same size") = [&]() {
					auto test_fn = [&]<typename Value8, std::size_t Rows8, std::size_t Columns8>() {
						auto matrix_1 = mpp::matrix<Value8, Rows8, Columns8>{};
						auto matrix_2 = mpp::matrix<Value8, mpp::dynamic, mpp::dynamic>{ Rows8, Columns8 };
						auto matrix_3 = mpp::matrix<Value8, Rows8, mpp::dynamic>{ Columns8 };
						auto matrix_4 = mpp::matrix<Value8, mpp::dynamic, Columns8>{ Rows8 };

						matrix_1 = range_2d;
						matrix_2 = range_2d;
						matrix_3 = range_2d;
						matrix_4 = range_2d;

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows8, Columns8);

						matrix_1.assign(range_2d);
						matrix_2.assign(range_2d);
						matrix_3.assign(range_2d);
						matrix_4.assign(range_2d);

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows8, Columns8);

						// Test rvalue overload
						matrix_1.assign(std::vector<std::vector<int>>{ { { 1, 2, 3 }, { 4, 5, 6 } } }, mpp::unsafe);
						matrix_2.assign(std::vector<std::vector<int>>{ { { 1, 2, 3 }, { 4, 5, 6 } } }, mpp::unsafe);
						matrix_3.assign(std::vector<std::vector<int>>{ { { 1, 2, 3 }, { 4, 5, 6 } } }, mpp::unsafe);
						matrix_4.assign(std::vector<std::vector<int>>{ { { 1, 2, 3 }, { 4, 5, 6 } } }, mpp::unsafe);

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows8, Columns8);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows8, Columns8);

						// Test std::array overloads for fully static matrices

						const auto range_2d_as_array =
							std::array<std::array<Value8, Columns8>, Rows8>{ { { 1, 2, 3 }, { 4, 5, 6 } } };

						matrix_1 = range_2d_as_array;
						compare_matrix_to_range_2d(matrix_1, range_2d_as_array, Rows8, Columns8);

						matrix_1.assign(range_2d_as_array);
						compare_matrix_to_range_2d(matrix_1, range_2d_as_array, Rows8, Columns8);

						matrix_1.assign(std::array<std::array<Value8, Columns8>, Rows8>{ { { 1, 2, 3 }, { 4, 5, 6 } } },
							mpp::unsafe);
						compare_matrix_to_range_2d(matrix_1, range_2d_as_array, Rows8, Columns8);
					};

					test_fn.template operator()<int, 2, 3>();
					test_fn.template operator()<float, 2, 3>(); // Test different value type assignment
				};

				given("A 2D initializer_list") = [&]() {
					const auto initializer_list_2d =
						std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

					auto test_fn = [&]<typename Value6, std::size_t Rows6, std::size_t Columns6>() {
						auto matrix_1 = mpp::matrix<Value6, Rows6, Columns6>{};
						auto matrix_2 = mpp::matrix<Value6, mpp::dynamic, mpp::dynamic>{ Rows6, Columns6 };
						auto matrix_3 = mpp::matrix<Value6, Rows6, mpp::dynamic>{ Columns6 };
						auto matrix_4 = mpp::matrix<Value6, mpp::dynamic, Columns6>{ Rows6 };

						matrix_1 = initializer_list_2d;
						matrix_2 = initializer_list_2d;
						matrix_3 = initializer_list_2d;
						matrix_4 = initializer_list_2d;

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows6, Columns6);

						matrix_1.assign(initializer_list_2d);
						matrix_2.assign(initializer_list_2d);
						matrix_3.assign(initializer_list_2d);
						matrix_4.assign(initializer_list_2d);

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows6, Columns6);

						// Test rvalue overload
						matrix_1.assign(std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } },
							mpp::unsafe);
						matrix_2.assign(std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } },
							mpp::unsafe);
						matrix_3.assign(std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } },
							mpp::unsafe);
						matrix_4.assign(std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } },
							mpp::unsafe);

						compare_matrix_to_range_2d(matrix_1, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_2, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_3, range_2d, Rows6, Columns6);
						compare_matrix_to_range_2d(matrix_4, range_2d, Rows6, Columns6);
					};

					test_fn.template operator()<int, 2, 3>();
					test_fn.template operator()<float, 2, 3>(); // Test different value type assignment
				};

				given("A matrix") = [&]() {
					auto test_fn =
						[&]<typename LeftValue7, typename RightValue7, std::size_t Rows7, std::size_t Columns7>() {
							const auto matrix = mpp::matrix<LeftValue7, Rows7, Columns7>{ range_2d };
							auto matrix_1     = mpp::matrix<RightValue7, Rows7, Columns7>{};
							auto matrix_2     = mpp::matrix<RightValue7, mpp::dynamic, mpp::dynamic>{ Rows7, Columns7 };
							auto matrix_3     = mpp::matrix<RightValue7, Rows7, mpp::dynamic>{ Columns7 };
							auto matrix_4     = mpp::matrix<RightValue7, mpp::dynamic, Columns7>{ Rows7 };

							matrix_1 = matrix;
							matrix_2 = matrix;
							matrix_3 = matrix;
							matrix_4 = matrix;

							compare_matrix_to_matrix(matrix, matrix_1);
							compare_matrix_to_matrix(matrix, matrix_2);
							compare_matrix_to_matrix(matrix, matrix_3);
							compare_matrix_to_matrix(matrix, matrix_4);

							matrix_1.assign(matrix);
							matrix_2.assign(matrix);
							matrix_3.assign(matrix);
							matrix_4.assign(matrix);

							compare_matrix_to_matrix(matrix, matrix_1);
							compare_matrix_to_matrix(matrix, matrix_2);
							compare_matrix_to_matrix(matrix, matrix_3);
							compare_matrix_to_matrix(matrix, matrix_4);

							// Test rvalue overload
							matrix_1.assign(mpp::matrix{ range_2d }, mpp::unsafe);
							matrix_2.assign(mpp::matrix{ range_2d }, mpp::unsafe);
							matrix_3.assign(mpp::matrix{ range_2d }, mpp::unsafe);
							matrix_4.assign(mpp::matrix{ range_2d }, mpp::unsafe);

							compare_matrix_to_matrix(matrix, matrix_1);
							compare_matrix_to_matrix(matrix, matrix_2);
							compare_matrix_to_matrix(matrix, matrix_3);
							compare_matrix_to_matrix(matrix, matrix_4);
						};

					test_fn.template operator()<int, int, 2, 3>();   // @NOTE: Also covers defaulted operator=
					test_fn.template operator()<int, float, 2, 3>(); // Test different value type assignment
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
					const auto matrix   = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
					const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ matrix };

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

				scenario("Copy constructing from another matrix with custom allocator") = [&]() {
					const auto allocator = custom_allocator<int>{};

					{
						const auto matrix =
							mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ matrix, allocator };

						compare_matrix_to_matrix(matrix, matrix_2);
					}

					{
						const auto matrix = mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ matrix, allocator };

						compare_matrix_to_matrix(matrix, matrix_2);
					}

					{
						const auto matrix = mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ matrix, allocator };

						compare_matrix_to_matrix(matrix, matrix_2);
					}
				};
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

				scenario("Move constructing from another matrix with custom allocator") = [&]() {
					const auto allocator = custom_allocator<int>{};

					{
						auto matrix = mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, mpp::dynamic, mpp::dynamic, custom_allocator<int>>{ std::move(matrix),
								allocator };

						compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
					}

					{
						auto matrix = mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, mpp::dynamic, 3, custom_allocator<int>>{ std::move(matrix), allocator };

						compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
					}

					{
						auto matrix = mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ range_2d };
						const auto matrix_2 =
							mpp::matrix<int, 2, mpp::dynamic, custom_allocator<int>>{ std::move(matrix), allocator };

						compare_matrix_to_range_2d(matrix_2, range_2d, 2, 3);
					}
				};
			};

			// @NOTE: Copy and move assigning matrices is already covered in above assignment test

			scenario("Matrices should be destructible") = [&]() {
				expect(constant<std::is_destructible_v<mpp::matrix<int, 2, 3>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, 3>>>);
				expect(constant<std::is_destructible_v<mpp::matrix<int, 2, mpp::dynamic>>>);
			};
		};
	};
}
