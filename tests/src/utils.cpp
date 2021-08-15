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

#include "../include/utils.hpp"

#include <compare>
#include <concepts>
#include <cstddef>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

namespace
{
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
	feature("Elements comparison") = []() {
		test_cmp_elems<join_mats<all_mats<int, 0, 0>, all_mats<int, 0, 0>>, std::strong_ordering>(
			"utils/cmp_elems/0x0_0x0.txt");
		test_cmp_elems<join_mats<all_mats<int, 1, 1>, all_mats<int, 1, 1>>, std::strong_ordering>(
			"utils/cmp_elems/1x1_1x1.txt");
		test_cmp_elems<join_mats<all_mats<int, 3, 3>, all_mats<int, 3, 3>>, std::strong_ordering>(
			"utils/cmp_elems/3x3_3x3.txt");
		test_cmp_elems<join_mats<all_mats<float, 4, 4>, all_mats<float, 4, 4>>, std::partial_ordering>(
			"utils/cmp_elems/4x4_4x4.txt");
	};

	return 0;
}
