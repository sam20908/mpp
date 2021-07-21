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

#include <mpp/util/cmp.hpp>
#include <mpp/util/type.hpp>
#include <mpp/algo.hpp>
#include <mpp/mat.hpp>

#include "../include/alloc.hpp"
#include "../include/utils.hpp"

#include <compare>
#include <string_view>
#include <type_traits>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

namespace
{
	template<typename Mats, typename To>
	void test_det(std::string_view test_name)
	{
		test(test_name.data()) = [test_name]<typename Mat>(std::type_identity<Mat>) {
			const auto [mat, num] = parse_test(test_name, parse_mat<Mat>, parse_val<To>);
			const auto out        = mpp::det(mat, std::type_identity<To>{});

			cmp_nums(out, num);
		} | Mats{};
	}

	template<typename Mats>
	void test_fn(std::string_view test_name, const auto& fn)
	{
		test(test_name.data()) = [&, test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			const auto [mat, mat2] = parse_test(test_name, parse_mat<Mat>, parse_mat<Mat2>);
			const auto out         = fn(mat, std::type_identity<Mat2>{});

			cmp_mat_types(out, mat2);
			cmp_mat_to_expr_like(out, mat2);
		} | Mats{};
	}

	template<typename Mats>
	void test_sub(std::string_view test_name, const auto& fn)
	{
		test(test_name.data()) =
			[&, test_name]<typename Mat, typename Mat2, typename Mat3>(
				std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>, std::type_identity<Mat3>>) {
				const auto [mat, mat2, mat3] = parse_test(test_name, parse_mat<Mat>, parse_mat<Mat2>, parse_mat<Mat3>);
				const auto out               = fn(mat, mat2, std::type_identity<Mat3>{});

				cmp_mat_types(out, mat3);
				cmp_mat_to_expr_like(out, mat3);
			} |
			Mats{};
	}

	template<typename Mats>
	void test_lu(std::string_view test_name)
	{
		test(test_name.data()) =
			[test_name]<typename Mat, typename Mat2, typename Mat3>(
				std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>, std::type_identity<Mat3>>) {
				const auto [mat, mat2, mat3] = parse_test(test_name, parse_mat<Mat>, parse_mat<Mat2>, parse_mat<Mat3>);
				const auto [out, out2]       = mpp::lu(mat, std::type_identity<Mat2>{}, std::type_identity<Mat3>{});

				scenario("Testing L matrix") = [&]() {
					cmp_mat_types(out, mat2);
					cmp_mat_to_expr_like(out, mat2);
				};

				scenario("Testing U matrix") = [&]() {
					cmp_mat_types(out2, mat3);
					cmp_mat_to_expr_like(out2, mat3);
				};
			} |
			Mats{};
	}

	template<typename Mats>
	void test_block(std::string_view test_name)
	{
		test(test_name.data()) = [test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			const auto [mat, idxs, mat2] = parse_test(test_name, parse_mat<Mat>, parse_block_idxs, parse_mat<Mat2>);
			const auto out =
				mpp::block(mat, idxs.row_start, idxs.col_start, idxs.row_end, idxs.col_end, std::type_identity<Mat2>{});

			cmp_mat_types(out, mat2);
			cmp_mat_to_expr_like(out, mat2);
		} | Mats{};
	}
} // namespace

int main()
{
	feature("Determinant") = []() {
		test_det<all_mats<int, 0, 0>, int>("algos/det/0x0.txt");
		test_det<all_mats<float, 1, 1>, double>("algos/det/1x1.txt");
		test_det<all_mats<float, 2, 2>, double>("algos/det/2x2.txt");
		test_det<all_mats<int, 3, 3>, int>("algos/det/3x3.txt");
		test_det<all_mats<double, 10, 10>, double>("algos/det/10x10.txt");
		test_det<all_mats<double, 20, 20>, double>("algos/det/20x20.txt");

		// Test different return type
		test_det<dyn_mat<float>, int>("algos/det/3x3.txt");
	};

	feature("Transpose") = []() {
		test_fn<join_mats<all_mats<float, 25, 25>, all_trps_mats<float, 25, 25>>>("algos/trps/25x25.txt", trps);
		test_fn<join_mats<all_mats<float, 50, 2>, all_trps_mats<float, 50, 2>>>("algos/trps/50x2.txt", trps);

		// Test different return type
		test_fn<join_mats<dyn_mat<float>, fixed_mat<double, 25, 25>>>("algos/trps/25x25.txt", trps);
	};

	feature("Inverse") = []() {
		test_fn<join_mats<all_mats<double, 0, 0>, all_mats<double, 0, 0>>>("algos/inv/0x0.txt", inverse);
		test_fn<join_mats<all_mats<double, 1, 1>, all_mats<double, 1, 1>>>("algos/inv/1x1.txt", inverse);
		test_fn<join_mats<all_mats<double, 2, 2>, all_mats<double, 2, 2>>>("algos/inv/2x2.txt", inverse);
		test_fn<join_mats<all_mats<int, 3, 3>, all_mats<int, 3, 3>>>("algos/inv/3x3_int.txt", inverse);
		test_fn<join_mats<all_mats<double, 3, 3>, all_mats<double, 3, 3>>>("algos/inv/3x3.txt", inverse);
		test_fn<join_mats<all_mats<double, 10, 10>, all_mats<double, 10, 10>>>("algos/inv/10x10.txt", inverse);

		// Test different return type
		test_fn<join_mats<dyn_mat<float>, fixed_mat<int, 3, 3>>>("algos/inv/3x3_int.txt", inverse);
	};

	feature("Forward substitution") = []() {
		test_sub<join_mats<all_mats<double, 4, 4>, all_mats<double, 4, 1>, all_mats<double, 4, 1>>>(
			"algos/fwd_sub/4x4_4x1.txt",
			fwd_sub);

		// Test different return type
		test_sub<join_mats<dyn_mat<double>, dyn_mat<double>, fixed_mat<int, 4, 1>>>("algos/fwd_sub/4x4_4x1.txt",
			fwd_sub);
	};

	feature("Backward substitution") = []() {
		test_sub<join_mats<all_mats<double, 3, 3>, all_mats<double, 3, 1>, all_mats<double, 3, 1>>>(
			"algos/back_sub/3x3_3x1.txt",
			back_sub);

		// Test different return type
		test_sub<join_mats<dyn_mat<int>, dyn_mat<int>, fixed_mat<float, 3, 1>>>("algos/back_sub/3x3_3x1.txt", back_sub);
	};

	feature("LU Decomposition") = []() {
		test_lu<join_mats<all_mats<double, 2, 2>, all_mats<double, 2, 2>, all_mats<double, 2, 2>>>("algos/lu/2x2.txt");
		test_lu<join_mats<all_mats<double, 3, 3>, all_mats<double, 3, 3>, all_mats<double, 3, 3>>>("algos/lu/3x3.txt");

		// Test different return type
		test_lu<join_mats<dyn_mat<double>, dyn_mat<double>, fixed_mat<float, 2, 2>>>("algos/lu/2x2.txt");
	};

	feature("Block") = []() {
		test_block<join_mats<all_mats<double, 3, 3>, all_mats<double, 1, 1>>>("algos/block/3x3_1x1_0_0_0_0.txt");
		test_block<join_mats<all_mats<double, 4, 4>, all_mats<double, 2, 2>>>("algos/block/4x4_2x2_2_2_3_3.txt");

		// Test different return type
		test_block<join_mats<dyn_mat<double>, fixed_mat<double, 1, 1>>>("algos/block/3x3_1x1_0_0_0_0.txt");
	};

	return 0;
}
