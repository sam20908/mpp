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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4459)
#endif

#include <boost/ut.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <mpp/matrix.hpp>

#include <memory>
#include <vector>

namespace boost::ut
{
	constexpr auto operator""_bf(const char*, decltype(sizeof("")))
	{
		return detail::integral_constant<false>{};
	}
} // namespace boost::ut

int main()
{
	// @NOTE: Comparisons are covered by utility_cpos.cpp

	using namespace boost::ut::bdd;
	using namespace boost::ut;

	feature("Destructible (part of rule of five)") = []() {
		expect(constant<std::is_destructible_v<mpp::matrix<int, 2, 3>>>);
		expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>>);
		expect(constant<std::is_destructible_v<mpp::matrix<int, mpp::dynamic, 3>>>);
		expect(constant<std::is_destructible_v<mpp::matrix<int, 2, mpp::dynamic>>>);
	};

	const auto range_2d = std::vector<std::vector<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };
	const auto matrix_1 = mpp::matrix<int, 2, 3>{ range_2d };
	const auto matrix_2 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
	const auto matrix_3 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
	const auto matrix_4 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };

	feature("operator()") = [&]() {
		expect(matrix_1(1, 1) == 5_i);
		expect(matrix_2(1, 1) == 5_i);
		expect(matrix_3(1, 1) == 5_i);
		expect(matrix_4(1, 1) == 5_i);
	};

	feature(".at()") = [&]() {
		expect(matrix_1.at(1, 1) == 5_i);
		expect(matrix_2.at(1, 1) == 5_i);
		expect(matrix_3.at(1, 1) == 5_i);
		expect(matrix_4.at(1, 1) == 5_i);
	};

	feature(".front()") = [&]() {
		given("We're only reading the data") = [&]() {
			expect(matrix_1.front() == 1_i);
			expect(matrix_2.front() == 1_i);
			expect(matrix_3.front() == 1_i);
			expect(matrix_4.front() == 1_i);
		};

		given("We're modifying the data") = [&]() {
			mut(matrix_1).front() = 2;
			mut(matrix_2).front() = 2;
			mut(matrix_3).front() = 2;
			mut(matrix_4).front() = 2;

			expect(matrix_1.front() == 2_i);
			expect(matrix_2.front() == 2_i);
			expect(matrix_3.front() == 2_i);
			expect(matrix_4.front() == 2_i);

			// Reset back to previous value for other tests
			mut(matrix_1).front() = 1;
			mut(matrix_2).front() = 1;
			mut(matrix_3).front() = 1;
			mut(matrix_4).front() = 1;
		};
	};

	feature(".back()") = [&]() {
		given("We're only reading the data") = [&]() {
			expect(matrix_1.back() == 6_i);
			expect(matrix_2.back() == 6_i);
			expect(matrix_3.back() == 6_i);
			expect(matrix_4.back() == 6_i);
		};

		given("We're modifying the data") = [&]() {
			mut(matrix_1).back() = 2;
			mut(matrix_2).back() = 2;
			mut(matrix_3).back() = 2;
			mut(matrix_4).back() = 2;

			expect(matrix_1.back() == 2_i);
			expect(matrix_2.back() == 2_i);
			expect(matrix_3.back() == 2_i);
			expect(matrix_4.back() == 2_i);

			// Reset back to previous value for other tests
			mut(matrix_1).back() = 6;
			mut(matrix_2).back() = 6;
			mut(matrix_3).back() = 6;
			mut(matrix_4).back() = 6;
		};
	};

	feature(".data()") = [&]() {
		given("We're only reading the data") = [&]() {
			expect(*matrix_1.data() == 1_i);
			expect(*matrix_2.data() == 1_i);
			expect(*matrix_3.data() == 1_i);
			expect(*matrix_4.data() == 1_i);
		};

		given("We're modifying the data") = [&]() {
			*mut(matrix_1).data() = 2;
			*mut(matrix_2).data() = 2;
			*mut(matrix_3).data() = 2;
			*mut(matrix_4).data() = 2;

			expect(*matrix_1.data() == 2_i);
			expect(*matrix_2.data() == 2_i);
			expect(*matrix_3.data() == 2_i);
			expect(*matrix_4.data() == 2_i);

			// Reset back to previous value for other tests
			*mut(matrix_1).data() = 1;
			*mut(matrix_2).data() = 1;
			*mut(matrix_3).data() = 1;
			*mut(matrix_4).data() = 1;
		};
	};

	feature(".size()") = [&]() {
		expect(matrix_1.size() == 6_ul);
		expect(matrix_2.size() == 6_ul);
		expect(matrix_3.size() == 6_ul);
		expect(matrix_4.size() == 6_ul);
	};

	feature(".empty()") = [&]() {
		expect(matrix_1.empty() == "false"_bf);
		expect(matrix_2.empty() == "false"_bf);
		expect(matrix_3.empty() == "false"_bf);
		expect(matrix_4.empty() == "false"_bf);
	};

	feature(".max_size()") = [&]() {
		// External factors can control max_size, so dynamic matrices don't have a "set" max_size
		expect(matrix_1.max_size() == 6_ul);
	};

	feature(".swap()") = [&]() {
		auto matrix_1_1 = mpp::matrix<int, 2, 3>{ range_2d };
		auto matrix_2_1 = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{ range_2d };
		auto matrix_3_1 = mpp::matrix<int, 2, mpp::dynamic>{ range_2d };
		auto matrix_4_1 = mpp::matrix<int, mpp::dynamic, 3>{ range_2d };

		// Too lazy to make a new 2D range
		matrix_1_1(0, 0) = 2;
		matrix_2_1(0, 0) = 2;
		matrix_3_1(0, 0) = 2;
		matrix_4_1(0, 0) = 2;

		mut(matrix_1).swap(matrix_1_1);
		mut(matrix_2).swap(matrix_2_1);
		mut(matrix_3).swap(matrix_3_1);
		mut(matrix_4).swap(matrix_4_1);

		expect(matrix_1(0, 0) == 2_i);
		expect(matrix_2(0, 0) == 2_i);
		expect(matrix_3(0, 0) == 2_i);
		expect(matrix_4(0, 0) == 2_i);

		// Swap back for next tests
		mut(matrix_1).swap(matrix_1_1);
		mut(matrix_2).swap(matrix_2_1);
		mut(matrix_3).swap(matrix_3_1);
		mut(matrix_4).swap(matrix_4_1);
	};

	feature(".get_allocator()") = [&]() {
		const auto allocator = std::allocator<int>{};

		expect(matrix_2.get_allocator() == allocator);
		expect(matrix_3.get_allocator() == allocator);
		expect(matrix_4.get_allocator() == allocator);
	};

	// This should be tested last because it destroys dynamic matrices
	feature(".clear()") = [&]() {
		mut(matrix_2).clear();
		mut(matrix_3).clear();
		mut(matrix_4).clear();

		expect(matrix_2.empty() == "true"_b);
		expect(matrix_3.empty() == "true"_b);
		expect(matrix_4.empty() == "true"_b);
	};

	return 0;
}