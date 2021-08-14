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

#include "../include/utils.hpp"

#include <initializer_list>
#include <vector>

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace mpp;

namespace
{
	template<typename Mats, bool Move>
	void test_assign_rng(std::string_view test_name)
	{
		test(test_name.data()) = [test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			auto [mat, vec2d, expected_mat] =
				parse_test(test_name, parse_mat<Mat>, parse_rng2d<typename Mat::value_type>, parse_mat<Mat2>);

			if constexpr (Move)
			{
				mat = std::move(vec2d);
			}
			else
			{
				mat = vec2d;
			}

			cmp_mat_to_expr_like(mat, expected_mat);
		} | Mats{};
	}

	template<typename Mats, bool Move>
	void test_assign_mat(std::string_view test_name)
	{
		test(test_name.data()) =
			[test_name]<typename Mat, typename Mat2, typename Mat3>(
				std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>, std::type_identity<Mat3>>) {
				auto [mat, mat2, expected_mat] =
					parse_test(test_name, parse_mat<Mat>, parse_mat<Mat2>, parse_mat<Mat3>);

				if constexpr (Move)
				{
					mat = std::move(mat2);
				}
				else
				{
					mat = mat2;
				}

				cmp_mat_to_expr_like(mat, expected_mat);
			} |
			Mats{};
	}
} // namespace


int main()
{
	// @NOTE: This covers copy and move assignment from rule of five (2/5)

	feature("Assigning a 2D range with same dimensions") = []() {
		using mats = join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>;

		test_assign_rng<mats, false>("assign/2x3_same_dims.txt");
		test_assign_rng<mats, true>("assign/2x3_same_dims.txt");
	};

	feature("Assigning a matrix with same dimensions") = []() {
		using mats = join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>, all_mats<int, 2, 3>>;

		test_assign_mat<mats, false>("assign/2x3_same_dims.txt");
		test_assign_mat<mats, true>("assign/2x3_same_dims.txt");
	};

	feature("Assigning a matrix with same dimensions but different types") = []() {
		using mats = join_mats<all_mats<int, 2, 3>, all_mats_reverse<int, 2, 3>, all_mats<int, 2, 3>>;

		test_assign_mat<mats, false>("assign/2x3_same_dims.txt");
		test_assign_mat<mats, true>("assign/2x3_same_dims.txt");
	};

	feature("Expanding dynamic matrices by 2D range (dynamic matrices only)") = []() {
		using mats = join_mats<dyn_mat<int>, dyn_mat<int>>;

		test_assign_rng<mats, false>("assign/2x3_10x10_expand.txt");
		test_assign_rng<mats, true>("assign/2x3_10x10_expand.txt");
		test_assign_rng<mats, false>("assign/2x3_10x3_expand.txt");
		test_assign_rng<mats, true>("assign/2x3_10x3_expand.txt");
		test_assign_rng<mats, false>("assign/2x3_2x10_expand.txt");
		test_assign_rng<mats, true>("assign/2x3_2x10_expand.txt");
	};

	feature("Shrinking dynamic matrices by 2D range (dynamic matrices only)") = []() {
		using mats = join_mats<dyn_mat<int>, dyn_mat<int>>;

		test_assign_rng<mats, false>("assign/10x10_2x3_shrink.txt");
		test_assign_rng<mats, true>("assign/10x10_2x3_shrink.txt");
		test_assign_rng<mats, false>("assign/10x3_2x3_shrink.txt");
		test_assign_rng<mats, true>("assign/10x3_2x3_shrink.txt");
		test_assign_rng<mats, false>("assign/2x10_2x3_shrink.txt");
		test_assign_rng<mats, true>("assign/2x10_2x3_shrink.txt");
	};

	feature("Expanding dynamic matrices by another matrix (dynamic matrices only)") = []() {
		using mats = join_mats<dyn_mat<int>, dyn_mat<int>, dyn_mat<int>>;

		test_assign_mat<mats, false>("assign/2x3_10x10_expand.txt");
		test_assign_mat<mats, true>("assign/2x3_10x10_expand.txt");
		test_assign_mat<mats, false>("assign/2x3_10x3_expand.txt");
		test_assign_mat<mats, true>("assign/2x3_10x3_expand.txt");
		test_assign_mat<mats, false>("assign/2x3_2x10_expand.txt");
		test_assign_mat<mats, true>("assign/2x3_2x10_expand.txt");
	};

	feature("Shrinking dynamic matrices by another matrix (dynamic matrices only)") = []() {
		using mats = join_mats<dyn_mat<int>, dyn_mat<int>, dyn_mat<int>>;

		test_assign_mat<mats, false>("assign/10x10_2x3_shrink.txt");
		test_assign_mat<mats, true>("assign/10x10_2x3_shrink.txt");
		test_assign_mat<mats, false>("assign/10x3_2x3_shrink.txt");
		test_assign_mat<mats, true>("assign/10x3_2x3_shrink.txt");
		test_assign_mat<mats, false>("assign/2x10_2x3_shrink.txt");
		test_assign_mat<mats, true>("assign/2x10_2x3_shrink.txt");
	};

	return 0;
}
