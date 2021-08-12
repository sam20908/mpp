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

#include <mpp/util/cmp.hpp>
#include <mpp/mat.hpp>

#include "../include/alloc.hpp"
#include "../include/utils.hpp"

#include <array>
#include <compare>
#include <cstddef>
#include <initializer_list>
#include <string_view>
#include <tuple>
#include <vector>

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace mpp;

namespace
{
	template<typename Mats>
	void test_init(std::string_view test_name, const auto& input_fn, const auto&... args)
	{
		test(test_name.data()) = [&, test_name]<typename Mat>(std::type_identity<Mat> identity) {
			const auto [out, expected_rng] =
				parse_test(test_name, input_fn(identity, args...), parse_vec2d<typename Mat::value_type>);

			cmp_mat_to_rng(out, expected_rng);
		} | Mats{};
	}

	template<typename Mats, bool Move>
	void test_init_copy_move_ctor(std::string_view test_name)
	{
		test(test_name.data()) = [&, test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			auto [mat, expected_rng] = parse_test(test_name, parse_mat<Mat>, parse_vec2d<typename Mat::value_type>);

			if constexpr (Move)
			{
				Mat2 out{ std::move(mat) };
				cmp_mat_to_rng(out, expected_rng);
			}
			else
			{
				Mat2 out{ mat };
				cmp_mat_to_rng(out, expected_rng);
			}
		} | Mats{};
	}
} // namespace

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests
	// @NOTE: This covers copy and move construction from rule of five (2/5)

	feature("Default initialization") = []() {
		test_init<all_mats<int, 0, 0>>("init/0x0_default.txt", parse_mat_construct_args);
	};

	feature("Copy initialization") = []() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, false>(
			"init/2x3_init_copy_and_move.txt");
	};

	feature("Copy initialization with different matrix type") = []() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats_reverse<int, 2, 3>>, false>(
			"init/2x3_init_copy_and_move.txt");
	};

	feature("Move initialization") = []() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, true>(
			"init/2x3_init_copy_and_move.txt");
	};

	feature("Move initialization with different matrix type") = []() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats_reverse<int, 2, 3>>, true>(
			"init/2x3_init_copy_and_move.txt");
	};

	feature("2D initializer list initialization") = []() {
		const auto init = std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		test_init<all_mats<int, 2, 3>>("init/2x3_init_list.txt", parse_mat_construct_args, init);
	};

	feature("2D range copy initialization") = []() {
		test_init<all_mats<int, 2, 3>>("init/2x3_rng_2d.txt", parse_mat_construct_rng2d<false>);
	};

	feature("2D range move initialization") = []() {
		test_init<all_mats<int, 2, 3>>("init/2x3_rng_2d.txt", parse_mat_construct_rng2d<true>);
	};

	feature("1D range copy initialization") = []() {
		test_init<all_mats<int, 2, 3>>("init/2x3_rng_1d.txt", parse_mat_construct_rng1d<false>);
	};

	feature("1D range move initialization") = []() {
		test_init<all_mats<int, 2, 3>>("init/2x3_rng_1d.txt", parse_mat_construct_rng1d<true>);
	};

	feature("Initialization via callable return values") = [&]() {
		const auto fn = []() {
			return 1;
		};

		test_init<all_mats<int, 2, 3>>("init/2x3_callable.txt", parse_mat_construct_args, 2ull, 3ull, fn);
	};

	feature("Initialization via value") = []() {
		const auto val = 2;

		test_init<all_mats<int, 2, 3>>("init/2x3_val.txt", parse_mat_construct_args, 2ull, 3ull, val);
	};

	feature("Initialization of identity matrices") = []() {
		test_init<all_mats<int, 3, 3>>("init/3x3_identity.txt", parse_mat_construct_args, 3ull, 3ull, identity);
	};

	return 0;
}
