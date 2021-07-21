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

#include <boost/ut.hpp>

#include <mpp/mat.hpp>
#include <mpp/util.hpp>

#include "../include/test_utilities.hpp"

#include <compare>
#include <concepts>
#include <cstddef>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

namespace
{
	template<typename Mats>
	void test_fn(std::string_view test_name, const auto& fn, const auto& parse_fn)
	{
		test(test_name.data()) = [&, test_name]<typename Mat>(std::type_identity<Mat>) {
			const auto [mat, expected_val] = parse_test(test_name, parse_mat<Mat>, parse_fn);
			const auto out                 = fn(mat);

			expect(out == expected_val);
		} | Mats{};
	}

	template<typename Mats, typename Order, typename Order2>
	void test_cmp_size(std::string_view test_name)
	{
		test(test_name.data()) = [test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			const auto [mat, mat2, cmp_rows, cmp_cols, expected_row_cmp, expected_col_cmp] = parse_test(test_name,
				parse_mat<Mat>,
				parse_mat<Mat2>,
				parse_val<bool>,
				parse_val<bool>,
				parse_ordering<Order>,
				parse_ordering<Order2>);

			const auto out = size_compare(mat, mat2, cmp_rows, cmp_cols);

			expect(out.first == expected_row_cmp);
			expect(out.second == expected_col_cmp);
		} | Mats{};
	}

	template<typename Mats, typename Order>
	void test_cmp_elems(std::string_view test_name)
	{
		test(test_name.data()) = [&, test_name]<typename Mat, typename Mat2>(
									 std::tuple<std::type_identity<Mat>, std::type_identity<Mat2>>) {
			const auto [mat, mat2, expected_cmp] =
				parse_test(test_name, parse_mat<Mat>, parse_mat<Mat2>, parse_ordering<Order>);

			const auto out = cmp(mat, mat2, cmp_fn);

			expect(out == expected_cmp);
		} | Mats{};
	}
} // namespace

int main()
{
	feature("Type") = []() {
		test_fn<fixed_mat<int, 0, 0>>("utilities/type/fixed.txt", mpp::type, parse_val<mat_type>);
		test_fn<dyn_mat<int>>("utilities/type/dyn.txt", mpp::type, parse_val<mat_type>);
		test_fn<dyn_rows_mat<int, 0>>("utilities/type/dyn_rows.txt", mpp::type, parse_val<mat_type>);
		test_fn<dyn_cols_mat<int, 0>>("utilities/type/dyn_cols.txt", mpp::type, parse_val<mat_type>);
	};

	feature("Square") = []() {
		test_fn<all_mats<int, 1, 1>>("utilities/sq/1x1.txt", sq, parse_val<bool>);
		test_fn<all_mats<int, 1, 2>>("utilities/sq/1x2.txt", sq, parse_val<bool>);
		test_fn<all_mats<int, 3, 3>>("utilities/sq/3x3.txt", sq, parse_val<bool>);
		test_fn<all_mats<int, 3, 2>>("utilities/sq/3x2.txt", sq, parse_val<bool>);
	};

	feature("Singular") = []() {
		test_fn<fixed_mat<int, 0, 0>>("utilities/sg/0x0.txt", sg, parse_val<bool>);
		test_fn<dyn_mat<int>>("utilities/sg/1x1.txt", sg, parse_val<bool>);
		test_fn<dyn_rows_mat<int, 2>>("utilities/sg/2x2.txt", sg, parse_val<bool>);
		test_fn<dyn_cols_mat<int, 3>>("utilities/sg/3x3.txt", sg, parse_val<bool>);
	};

	feature("Size comparison") = []() {
		test_cmp_size<join_mats<all_mats<int, 0, 0>, all_mats<int, 0, 0>>,
			std::partial_ordering,
			std::partial_ordering>("utilities/cmp_size/0x0_0x0.txt");
		test_cmp_size<join_mats<all_mats<int, 1, 1>, all_mats<int, 1, 2>>, std::strong_ordering, std::strong_ordering>(
			"utilities/cmp_size/1x1_1x2.txt");
		test_cmp_size<join_mats<all_mats<int, 3, 4>, all_mats<int, 3, 3>>, std::partial_ordering, std::strong_ordering>(
			"utilities/cmp_size/3x4_3x3.txt");
		test_cmp_size<join_mats<all_mats<int, 4, 4>, all_mats<int, 4, 4>>, std::strong_ordering, std::strong_ordering>(
			"utilities/cmp_size/4x4_4x4.txt");
	};

	feature("Elements comparison") = []() {
		test_cmp_elems<join_mats<all_mats<int, 0, 0>, all_mats<int, 0, 0>>, std::strong_ordering>(
			"utilities/cmp_elems/0x0_0x0.txt");
		test_cmp_elems<join_mats<all_mats<int, 1, 1>, all_mats<int, 1, 1>>, std::strong_ordering>(
			"utilities/cmp_elems/1x1_1x1.txt");
		test_cmp_elems<join_mats<all_mats<int, 3, 3>, all_mats<int, 3, 3>>, std::strong_ordering>(
			"utilities/cmp_elems/3x3_3x3.txt");
		test_cmp_elems<join_mats<all_mats<float, 4, 4>, all_mats<float, 4, 4>>, std::partial_ordering>(
			"utilities/cmp_elems/4x4_4x4.txt");
	};

	return 0;
}
