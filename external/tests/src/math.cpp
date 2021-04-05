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
	// Next free template parameter suffix is 5

	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

	"Math computations"_test = [&]() {
		scenario("Adding matrices") = [&]() {
			scenario("Adding matrices without evaluating the full result") = [&]() {
				auto test_fn = [&]<typename Value, std::size_t Rows, std::size_t Columns>() {
					const auto matrix_1 = mpp::matrix<Value, Rows, Columns>{ range_2d };
					const auto matrix_2 = mpp::matrix<Value, Rows, Columns>{ range_2d };

					const auto expr_obj = matrix_1 + matrix_2;

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
				auto test_fn = [&]<typename Value2, std::size_t Rows2, std::size_t Columns2>(auto... additional_args) {
					const auto matrix_1  = mpp::matrix<Value2, Rows2, Columns2>{ range_2d, additional_args... };
					const auto matrix_2  = mpp::matrix<Value2, Rows2, Columns2>{ range_2d, additional_args... };
					const auto result_2d = std::vector<std::vector<int>>{ { 2, 4, 6 }, { 8, 10, 12 } };

					const auto result = mpp::matrix{ matrix_1 + matrix_2 };

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
		};

		scenario("Subtracting matrices") = [&]() {
			scenario("Subtracting matrices without evaluating the full result") = [&]() {
				auto test_fn = [&]<typename Value3, std::size_t Rows3, std::size_t Columns3>() {
					const auto matrix_1 = mpp::matrix<Value3, Rows3, Columns3>{ range_2d };
					const auto matrix_2 = mpp::matrix<Value3, Rows3, Columns3>{ range_2d };

					const auto expr_obj = matrix_1 - matrix_2;

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
					const auto matrix_1  = mpp::matrix<Value4, Rows4, Columns4>{ range_2d, additional_args... };
					const auto matrix_2  = mpp::matrix<Value4, Rows4, Columns4>{ range_2d, additional_args... };
					const auto result_2d = std::vector<std::vector<int>>{ { 0, 0, 0 }, { 0, 0, 0 } };

					const auto result = mpp::matrix{ matrix_1 - matrix_2 };

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
		};
	};

	return 0;
}
