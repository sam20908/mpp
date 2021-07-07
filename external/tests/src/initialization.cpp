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

#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_utilities.hpp"

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
		test(test_name.data()) =
			[&, test_name]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
				std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>> identity) {
				const auto [out, expected_rng] = parse_test(test_name, input_fn(identity, args...), parse_vec2d<T>);

				cmp_mat_to_rng(out, expected_rng);
			} |
			Mats{};
	}

	template<typename Mats, bool Move>
	void test_init_copy_move_ctor(std::string_view test_name, const auto&... args)
	{
		test(test_name.data()) = [&, test_name]<typename T,
									 typename T2,
									 std::size_t RowsExtent,
									 std::size_t RowsExtent2,
									 std::size_t ColumnsExtent,
									 std::size_t ColumnsExtent2,
									 typename Alloc,
									 typename Alloc2>(
									 std::tuple<std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
										 std::type_identity<matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>>>) {
			using mat_t  = matrix<T, RowsExtent, ColumnsExtent, Alloc>;
			using mat2_t = matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>;

			auto [mat, expected_rng] = parse_test(test_name, parse_mat<mat_t>, parse_vec2d<T>);

			if constexpr (Move)
			{
				mat2_t out{ std::move(mat), args... };
				cmp_mat_to_rng(out, expected_rng);
			}
			else
			{
				mat2_t out{ mat, args... };
				cmp_mat_to_rng(out, expected_rng);
			}
		} | Mats{};
	}
} // namespace

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests
	// @NOTE: This covers copy and move construction from rule of five (2/5)

	using alloc_t        = custom_allocator<double>;
	const auto alloc_obj = alloc_t{};

	feature("Default initialization") = [&]() {
		test_init<all_mats<int, 0, 0>>("initialization/0x0_default.txt", parse_mat_construct_args);
	};

	feature("Allocator default initialization") = [&]() {
		test_init<dyn_mats<double, 0, 0, alloc_t>>("initialization/0x0_default.txt",
			parse_mat_construct_args,
			alloc_obj);
	};

	feature("Copy initialization") = [&]() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, false>(
			"initialization/2x3_init_copy_and_move.txt");
		test_init_copy_move_ctor<join_mats<dyn_mats<double, 2, 3, alloc_t>, dyn_mats<double, 2, 3, alloc_t>>, false>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj);

		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats_reverse<int, 2, 3>>, false>(
			"initialization/2x3_init_copy_and_move.txt");
		test_init_copy_move_ctor<join_mats<dyn_mats<double, 2, 3, alloc_t>, dyn_mats_reverse<double, 2, 3, alloc_t>>,
			false>("initialization/2x3_init_copy_and_move.txt", alloc_obj);
	};

	feature("Move initialization") = [&]() {
		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, true>(
			"initialization/2x3_init_copy_and_move.txt");
		test_init_copy_move_ctor<join_mats<dyn_mats<double, 2, 3, alloc_t>, dyn_mats<double, 2, 3, alloc_t>>, true>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj);

		test_init_copy_move_ctor<join_mats<all_mats<int, 2, 3>, all_mats_reverse<int, 2, 3>>, true>(
			"initialization/2x3_init_copy_and_move.txt");
		test_init_copy_move_ctor<join_mats<dyn_mats<double, 2, 3, alloc_t>, dyn_mats_reverse<double, 2, 3, alloc_t>>,
			true>("initialization/2x3_init_copy_and_move.txt", alloc_obj);
	};

	feature("2D initializer list initialization") = [&]() {
		const auto init = std::initializer_list<std::initializer_list<int>>{ { 1, 2, 3 }, { 4, 5, 6 } };

		test_init<all_mats<int, 2, 3>>("initialization/2x3_init_list.txt", parse_mat_construct_args, init);
		test_init<dyn_mats<double, 2, 3, alloc_t>>("initialization/2x3_init_list.txt",
			parse_mat_construct_args,
			init,
			alloc_obj);
	};

	feature("2D range initialization") = [&]() {
		test_init<all_mats<int, 2, 3>>("initialization/2x3_rng_2d.txt", parse_mat_construct_rng2d);
		test_init<dyn_mats<double, 2, 3, alloc_t>>("initialization/2x3_rng_2d.txt",
			parse_mat_construct_rng2d,
			alloc_obj);
	};

	feature("2D array initialization (fixed matrices only)") = [&]() {
		test_init<fixed_mat<int, 2, 3>>("initialization/2x3_arr2d.txt", parse_mat_construct_arr2d<2, 3>);
	};

	feature("1D range initialization") = [&]() {
		test_init<all_mats<int, 2, 3>>("initialization/2x3_rng_1d.txt", parse_mat_construct_rng1d<false>);
		test_init<dyn_mats<double, 2, 3, alloc_t>>("initialization/2x3_rng_1d.txt",
			parse_mat_construct_rng1d<false>,
			alloc_obj);
	};

	feature("Initialization via callable return values") = [&]() {
		const auto fn = []() {
			return 1.0;
		};

		test_init<all_mats<double, 2, 3>>("initialization/2x3_callable.txt", parse_mat_construct_val_arg, fn);
		test_init<dyn_mats<double, 2, 3, alloc_t>>("initialization/2x3_callable.txt",
			parse_mat_construct_val_arg,
			fn,
			alloc_obj);
	};

	feature("Initialization via value") = [&]() {
		const auto val = 2.0;

		test_init<all_mats<double, 2, 3>>("initialization/2x3_val.txt", parse_mat_construct_val_arg, val);
		test_init<dyn_mats<double, 2, 3, alloc_t>>("initialization/2x3_val.txt",
			parse_mat_construct_val_arg,
			val,
			alloc_obj);
	};

	feature("Initialization of identity matrices") = [&]() {
		test_init<all_mats<int, 3, 3>>("initialization/3x3_identity.txt", parse_mat_construct_val_arg, identity);
		test_init<dyn_mats<double, 3, 3, alloc_t>>("initialization/3x3_identity.txt",
			parse_mat_construct_val_arg,
			identity,
			0.0,
			1.0,
			alloc_obj);
	};

	return 0;
}
