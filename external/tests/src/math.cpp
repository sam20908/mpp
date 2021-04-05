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

#include <mpp/arithmetic.hpp>
#include <mpp/matrix.hpp>

#include "../../include/utility.hpp"
#include "../../thirdparty/ut.hpp"

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

int main()
{
	// Next free template parameter suffix is 11 

	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

	"Math computations"_test = [&]() {
		scenario("Adding matrices") = [&]() {
			scenario("Adding matrices without evaluating the full result") = [&]() {
				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>() {
					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ range_2d };
					const auto matrix_2 = mpp::matrix<Value, Rows, Columns>{ range_2d };

					const auto expr_obj = matrix_1 + matrix_2;

					expect(expr_obj.rows() == 2_ul);
					expect(expr_obj.columns() == 3_ul);
					expect(expr_obj(0, 0) == 2_i);
					expect(expr_obj(0, 1) == 4_i);
					expect(expr_obj(0, 2) == 6_i);
					expect(expr_obj(1, 0) == 8_i);
					expect(expr_obj(1, 1) == 10_i);
					expect(expr_obj(1, 2) == 12_i);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};

			scenario("Adding matrices and evaluate the full result") = [&]() {
				auto test_fn = [&]<typename Value8, std::size_t Rows8, std::size_t Columns8>(auto... additional_args) {
					const auto matrix_1  = mpp::matrix<Value8, Rows8, Columns8>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value8, Rows8, Columns8>{ range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 2, 4, 6 }, { 8, 10, 12 } };

					const auto result = mpp::matrix{ matrix_1 + matrix_2, additional_args... };

					compare_matrix_to_range_2d(result, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();

				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, 2, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, 3>(mpp::unsafe);
			};

			scenario("Adding onto existing matrix") = [&]() {
				auto test_fn = [&]<typename Value7, std::size_t Rows7, std::size_t Columns7>() {
					auto matrix_1        = mpp::matrix<Value7, Rows7, Columns7>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value7, Rows7, Columns7>{ range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 2, 4, 6 }, { 8, 10, 12 } };

					matrix_1 += matrix_2;

					compare_matrix_to_range_2d(matrix_1, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};

			scenario("Adding onto existing matrix with an expression object") = [&]() {
				const auto empty_range_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

				auto test_fn = [&]<typename Value9, std::size_t Rows9, std::size_t Columns9>() {
					auto matrix_1        = mpp::matrix<Value9, Rows9, Columns9>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value9, Rows9, Columns9>{ range_2d };
					const auto matrix_3  = mpp::matrix<Value9, Rows9, Columns9>{ empty_range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 2, 4, 6 }, { 8, 10, 12 } };

					matrix_1 += matrix_2 + matrix_3;

					compare_matrix_to_range_2d(matrix_1, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};
		};

		scenario("Subtracting matrices") = [&]() {
			scenario("Subtracting matrices without evaluating the full result") = [&]() {
				auto test_fn = [&]<typename Value3, std::size_t Rows3, std::size_t Columns3>() {
					const auto matrix_1 = mpp::matrix<Value3, Rows3, Columns3>{ range_2d };
					const auto matrix_2 = mpp::matrix<Value3, Rows3, Columns3>{ range_2d };

					const auto expr_obj = matrix_1 - matrix_2;

					expect(expr_obj.rows() == 2_ul);
					expect(expr_obj.columns() == 3_ul);
					expect(expr_obj(0, 0) == 0_i);
					expect(expr_obj(0, 1) == 0_i);
					expect(expr_obj(0, 2) == 0_i);
					expect(expr_obj(1, 0) == 0_i);
					expect(expr_obj(1, 1) == 0_i);
					expect(expr_obj(1, 2) == 0_i);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};

			scenario("Subtracting matrices and evaluate the full result") = [&]() {
				auto test_fn = [&]<typename Value4, std::size_t Rows4, std::size_t Columns4>(auto... additional_args) {
					const auto matrix_1  = mpp::matrix<Value4, Rows4, Columns4>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value4, Rows4, Columns4>{ range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

					const auto result = mpp::matrix{ matrix_1 - matrix_2, additional_args... };

					compare_matrix_to_range_2d(result, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();

				test_fn.template operator()<int, 2, 3>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, 2, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, 3>(mpp::unsafe);
			};

			scenario("Subtracting onto existing matrix") = [&]() {
				auto test_fn = [&]<typename Value2, std::size_t Rows2, std::size_t Columns2>() {
					auto matrix_1        = mpp::matrix<Value2, Rows2, Columns2>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value2, Rows2, Columns2>{ range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

					matrix_1 -= matrix_2;

					compare_matrix_to_range_2d(matrix_1, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};

			scenario("Subtracting onto existing matrix with an expression object") = [&]() {
				const auto empty_range_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

				auto test_fn = [&]<typename Value10, std::size_t Rows10, std::size_t Columns10>() {
					auto matrix_1        = mpp::matrix<Value10, Rows10, Columns10>{ range_2d };
					const auto matrix_2  = mpp::matrix<Value10, Rows10, Columns10>{ range_2d };
					const auto matrix_3  = mpp::matrix<Value10, Rows10, Columns10>{ empty_range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

					matrix_1 -= matrix_2 - matrix_3;

					compare_matrix_to_range_2d(matrix_1, result_2d, 2, 3);
				};

				test_fn.template operator()<int, 2, 3>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3>();
			};
		};

		feature("Multiplication with matrices") = [&]() {
			scenario("Multiplying matrices without evaluating the full result") = [&]() {
				const auto left_range_2d  = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
				const auto right_range_2d = std::vector<std::vector<int>>{ { 3 }, { 6 }, { 9 } };

				auto test_fn = [&]<typename Value6,
								   std::size_t LeftRows6,
								   std::size_t LeftColumns6,
								   std::size_t RightRows6,
								   std::size_t RightColumns6>() {
					const auto matrix_1 = mpp::matrix<Value6, LeftRows6, LeftColumns6>{ left_range_2d };
					const auto matrix_2 = mpp::matrix<Value6, RightRows6, RightColumns6>{ right_range_2d };

					const auto expr_obj = matrix_1 * matrix_2;

					// Note that result is 2x1 [[42], [96]]
					expect(expr_obj.rows() == 2_ul);
					expect(expr_obj.columns() == 1_ul);
					expect(expr_obj(0, 0) == 42_i);
					expect(expr_obj(1, 0) == 96_i);
				};

				test_fn.template operator()<int, 2, 3, mpp::dynamic, 1>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic, 3, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3, 3, 1>();
			};

			scenario("Multiplying matrices and evaluate the full result") = [&]() {
				const auto left_range_2d  = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
				const auto right_range_2d = std::vector<std::vector<int>>{ { 3 }, { 6 }, { 9 } };

				auto test_fn = [&]<typename Value5,
								   std::size_t LeftRows5,
								   std::size_t LeftColumns5,
								   std::size_t RightRows5,
								   std::size_t RightColumns5>(auto... additional_args) {
					const auto matrix_1  = mpp::matrix<Value5, LeftRows5, LeftColumns5>{ left_range_2d };
					const auto matrix_2  = mpp::matrix<Value5, RightRows5, RightColumns5>{ right_range_2d };
					const auto result_2d = std::vector<std::vector<int>>{ { 42 }, { 96 } };

					const auto result = mpp::matrix{ matrix_1 * matrix_2, additional_args... };

					compare_matrix_to_range_2d(result, result_2d, 2, 1);
				};

				test_fn.template operator()<int, 2, 3, mpp::dynamic, 1>();
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic, 3, mpp::dynamic>();
				test_fn.template operator()<int, 2, mpp::dynamic, mpp::dynamic, mpp::dynamic>();
				test_fn.template operator()<int, mpp::dynamic, 3, 3, 1>();

				test_fn.template operator()<int, 2, 3, mpp::dynamic, 1>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, mpp::dynamic, 3, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, 2, mpp::dynamic, mpp::dynamic, mpp::dynamic>(mpp::unsafe);
				test_fn.template operator()<int, mpp::dynamic, 3, 3, 1>(mpp::unsafe);
			};
		};
	};

	return 0;
}
