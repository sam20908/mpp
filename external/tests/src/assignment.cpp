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

#include "../../include/test_parsers.hpp"
#include "../../include/utility.hpp"

#include <initializer_list>
#include <vector>

using namespace boost::ut::bdd;

template<typename Val, template<typename> typename Mat>
static void test_assign_rng(const std::string& file)
{
	const auto result = parse_test<temp_types<mat_t, vec2d_t, mat_t>, types<Val, Val, Val>>(get_filepath(file),
		std::tuple{ mat_fn, vec2d_fn, mat_fn });
	auto expand_rng   = std::get<1>(result);
	const auto end    = std::get<2>(result);

	when("Copy assignment") = [&]() {
		auto start = std::get<0>(result);
		start      = expand_rng;
		cmp_mat_to_expr_like(start, end);
	};

	when("Move assignment") = [&]() {
		auto start = std::get<0>(result);
		start      = std::move(expand_rng);
		cmp_mat_to_expr_like(start, end);
	};
}

template<typename Val>
static void test_assign_mat(const std::string& file)
{
	const auto result = parse_test<temp_types<vec2d_t, vec2d_t>, types<Val, Val>>(get_filepath(file),
		std::tuple{ vec2d_fn, vec2d_fn });

	const auto rng          = std::get<0>(result);
	const auto expected_rng = std::get<1>(result);
	auto tup                = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, rng));

	when("Copy assignment") = [&]() {
		const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																					 auto temp =
																						 mpp::matrix<int, 2, 3>{};
																					 temp = std::get<0>(tup);
																					 return temp;
																				 },
			[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
				temp      = std::get<1>(tup);
				return temp;
			},
			[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, 3>{};
				temp      = std::get<2>(tup);
				return temp;
			},
			[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, 2, mpp::dynamic>{};
				temp      = std::get<3>(tup);
				return temp;
			} }));

		for_each_in_tup(tup_copy, [&](const auto& mat) {
			cmp_mat_to_rng(mat, expected_rng);
		});
	};

	when("Move assignment") = [&]() {
		const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																					 auto temp =
																						 mpp::matrix<int, 2, 3>{};
																					 temp = std::move(std::get<0>(tup));
																					 return temp;
																				 },
			[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
				temp      = std::move(std::get<1>(tup));
				return temp;
			},
			[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, 3>{};
				temp      = std::move(std::get<2>(tup));
				return temp;
			},
			[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, 2, mpp::dynamic>{};
				temp      = std::move(std::get<3>(tup));
				return temp;
			} }));

		for_each_in_tup(tup_copy, [&](const auto& mat) {
			cmp_mat_to_rng(mat, expected_rng);
		});
	};
}

template<typename Val>
static void test_assign_mat_different(const std::string& file)
{
	// Similar to test_assign_mat_different but assigning to different kinds of matrices

	const auto result = parse_test<temp_types<vec2d_t, vec2d_t>, types<Val, Val>>(get_filepath(file),
		std::tuple{ vec2d_fn, vec2d_fn });

	const auto rng          = std::get<0>(result);
	const auto expected_rng = std::get<1>(result);
	auto tup                = create_mats<int, 2, 3, all_mats_t>(args(fwd_args, rng));

	when("Copy assignment") = [&]() {
		const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																					 auto temp =
																						 mpp::matrix<int, 2, 3>{};
																					 temp = std::get<3>(tup);
																					 return temp;
																				 },
			[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
				temp      = std::get<2>(tup);
				return temp;
			},
			[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, 3>{};
				temp      = std::get<1>(tup);
				return temp;
			},
			[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, 2, mpp::dynamic>{};
				temp      = std::get<0>(tup);
				return temp;
			} }));

		for_each_in_tup(tup_copy, [&](const auto& mat) {
			cmp_mat_to_rng(mat, expected_rng);
		});
	};

	when("Move assignment") = [&]() {
		const auto tup_copy = create_mats<int, 2, 3, all_mats_t>(args(overloaded{ [&](types<mpp::matrix<int, 2, 3>>) {
																					 auto temp =
																						 mpp::matrix<int, 2, 3>{};
																					 temp = std::move(std::get<3>(tup));
																					 return temp;
																				 },
			[&](types<mpp::matrix<int, mpp::dynamic, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, mpp::dynamic>{};
				temp      = std::move(std::get<2>(tup));
				return temp;
			},
			[&](types<mpp::matrix<int, mpp::dynamic, 3>>) {
				auto temp = mpp::matrix<int, mpp::dynamic, 3>{};
				temp      = std::move(std::get<1>(tup));
				return temp;
			},
			[&](types<mpp::matrix<int, 2, mpp::dynamic>>) {
				auto temp = mpp::matrix<int, 2, mpp::dynamic>{};
				temp      = std::move(std::get<0>(tup));
				return temp;
			} }));

		for_each_in_tup(tup_copy, [&](const auto& mat) {
			cmp_mat_to_rng(mat, expected_rng);
		});
	};
}

template<typename T>
using fixed_mat_t = mpp::matrix<T, 2, 3>;

template<typename T>
using dyn_mat_t = mpp::matrix<T>;

template<typename T>
using dyn_row_mat_t = mpp::matrix<T, mpp::dynamic, 3>;

template<typename T>
using dyn_column_mat_t = mpp::matrix<T, 2, mpp::dynamic>;

int main()
{
	// @NOTE: This covers copy and move assignment from rule of five (2/5)

	feature("Expanding dynamic matrices") = []() {
		test_assign_rng<int, dyn_mat_t>("assignment/expand_dyn_mat.txt");
		test_assign_rng<int, dyn_row_mat_t>("assignment/expand_dyn_row_mat.txt");
		test_assign_rng<int, dyn_column_mat_t>("assignment/expand_dyn_column_mat.txt");
	};

	feature("Expanding dynamic matrices") = []() {
		test_assign_rng<int, dyn_mat_t>("assignment/shrink_dyn_mat.txt");
		test_assign_rng<int, dyn_row_mat_t>("assignment/shrink_dyn_row_mat.txt");
		test_assign_rng<int, dyn_column_mat_t>("assignment/shrink_dyn_column_mat.txt");
	};

	feature("Assigning for fixed matrices") = []() {
		test_assign_rng<int, fixed_mat_t>("assignment/assign_fixed_mat.txt");
	};

	test_assign_mat<int>("assignment/assign_mat.txt");
	test_assign_mat_different<int>("assignment/assign_mat.txt");

	return 0;
}
