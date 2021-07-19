/***
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


#include <boost/ut.hpp>

#include <mpp/mat.hpp>

#include <memory>
#include <vector>

int main()
{
	// @NOTE: This destructible from rule of five (1/5)

	using namespace boost::ut::bdd;
	using namespace boost::ut;
	using namespace mpp;

	feature("Destructible (part of rule of five)") = []() {
		expect(constant<std::is_destructible_v<mat<int, 2, 3>>>);
		expect(constant<std::is_destructible_v<mat<int, dyn, dyn>>>);
		expect(constant<std::is_destructible_v<mat<int, dyn, 3>>>);
		expect(constant<std::is_destructible_v<mat<int, 2, dyn>>>);
	};

	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
	const auto mats     = std::tuple{ mat<int, 2, 3>{ range_2d },
        mat<int, dyn, dyn>{ range_2d },
        mat<int, 2, dyn>{ range_2d },
        mat<int, dyn, 3>{ range_2d } };
	const auto dyn_mats = std::tuple{ mat<int, dyn, dyn>{ range_2d },
		mat<int, 2, dyn>{ range_2d },
		mat<int, dyn, 3>{ range_2d } };

	feature("operator()") = [&]() {
		given("We're only reading the data") = [&](const auto& mat) {
			expect(mat(1, 1) == 5_i);
		} | mats;

		given("We're modifying the data") = [&](const auto& mat) {
			mut(mat)(1, 1) = 10;

			expect(mat(1, 1) == 10_i);

			// Reset values for following tests
			mut(mat)(1, 1) = 5;
		} | mats;
	};

	feature("operator[]") = [&]() {
		given("We're only reading the data") = [&](const auto& mat) {
			expect(mat[4] == 5_i);
		} | mats;

		given("We're modifying the data") = [&](const auto& mat) {
			mut(mat)[4] = 10;

			expect(mat[4] == 10_i);

			// Reset values for following tests
			mut(mat)[4] = 5;
		} | mats;
	};

	feature(".front()") = [&]() {
		given("We're only reading the data") = [&](const auto& mat) {
			expect(mat.front() == 1_i);
		} | mats;

		given("We're modifying the data") = [&](const auto& mat) {
			mut(mat).front() = 2;

			expect(mat.front() == 2_i);

			// Reset back to previous value for other tests
			mut(mat).front() = 1;
		} | mats;
	};

	feature(".back()") = [&]() {
		given("We're only reading the data") = [&](const auto& mat) {
			expect(mat.back() == 6_i);
		} | mats;

		given("We're modifying the data") = [&](const auto& mat) {
			mut(mat).back() = 2;

			expect(mat.back() == 2_i);

			// Reset back to previous value for other tests
			mut(mat).back() = 6;
		} | mats;
	};

	feature(".data()") = [&]() {
		given("We're only reading the data") = [&](const auto& mat) {
			expect(*mat.data() == 1_i);
		} | mats;

		given("We're modifying the data") = [&](const auto& mat) {
			*mut(mat).data() = 2;

			expect(*mat.data() == 2_i);

			// Reset back to previous value for other tests
			*mut(mat).data() = 1;
		} | mats;
	};

	feature(".size()") = [&](const auto& mat) {
		expect(mat.size() == 6_ul);
	} | mats;

	feature(".empty()") = [&](const auto& mat) {
		expect(mat.empty() != "true"_b);
	} | mats;

	feature(".max_size()") = [&]() {
		// External factors can control max_size, so dynamic matrices don't have a "set" max_size
		expect(std::get<0>(mats).max_size() == 6_ul);
	};

	feature(".swap()") = [&]<typename Mat>(const Mat& mat) {
		auto mat_1 = Mat{ range_2d };

		// Too lazy to make a new 2D range
		mat_1(0, 0) = 2;

		mut(mat).swap(mat_1);

		expect(mat(0, 0) == 2_i);

		// Swap back for next tests
		mut(mat).swap(mat_1);
	} | mats;

	const auto alloc = std::allocator<int>{};

	feature(".get_allocator()") = [&](const auto& mat) {
		expect(mat.get_allocator() == alloc);
	} | dyn_mats;

	feature(".clear()") = [&](const auto& mat) {
		mut(mat).clear();

		expect(mat.empty() == "true"_b);
	} | dyn_mats;

	return 0;
}
