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

#include <mpp/arithmetic.hpp>
#include <mpp/matrix.hpp>

#include "../../include/test_utilities.hpp"

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace mpp;

namespace
{
	template<typename Mats, bool ConstructMat>
	void test_op(std::string_view test_name, const auto& op)
	{
		test(test_name) = [&, test_name]<typename T,
							  typename T2,
							  typename T3,
							  std::size_t RowsExtent,
							  std::size_t RowsExtent2,
							  std::size_t RowsExtent3,
							  std::size_t ColumnsExtent,
							  std::size_t ColumnsExtent2,
							  std::size_t ColumnsExtent3,
							  typename Alloc,
							  typename Alloc2,
							  typename Alloc3>(
							  std::tuple<std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
								  std::type_identity<matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>>,
								  std::type_identity<matrix<T3, RowsExtent3, ColumnsExtent3, Alloc3>>>) {
			using mat_t  = matrix<T, RowsExtent, ColumnsExtent, Alloc>;
			using mat2_t = matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>;
			using mat3_t = matrix<T3, RowsExtent3, ColumnsExtent3, Alloc3>;

			const auto [mat, mat2, expected_mat] =
				parse_test(test_name, parse_mat<mat_t>, parse_mat<mat2_t>, parse_mat<mat3_t>);

			const auto out = [&]() {
				if constexpr (ConstructMat)
				{
					return mat3_t{ op(mat, mat2) };
				}
				else
				{
					return op(mat, mat2);
				}
			}();

			if constexpr (ConstructMat)
			{
				cmp_mat_types(out, expected_mat);
			}

			cmp_mat_to_expr_like(out, expected_mat);
		} | Mats{};
	}

	template<typename Mats, bool ConstructMat>
	void test_num_op(const std::string& test_name, const auto& op)
	{
		test(test_name) = [&, test_name]<typename T,
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

			const auto [mat, val, expected_mat] =
				parse_test(test_name, parse_mat<mat_t>, parse_val<T>, parse_mat<mat2_t>);

			const auto out = [&]() {
				if constexpr (ConstructMat)
				{
					return mat2_t{ op(mat, val) };
				}
				else
				{
					return op(mat, val);
				}
			}();

			if constexpr (ConstructMat)
			{
				cmp_mat_types(out, expected_mat);
			}

			cmp_mat_to_expr_like(out, expected_mat);
		} | Mats{};
	}
} // namespace

int main()
{
	feature("Addition") = []() {
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>, all_mats<int, 2, 3>>, false>(
			"arithmetic/2x3_add.txt",
			std::plus{});
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>, all_mats<int, 2, 3>>, true>(
			"arithmetic/2x3_add.txt",
			std::plus{});
	};

	feature("Subtraction") = []() {
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>, all_mats<int, 2, 3>>, false>(
			"arithmetic/2x3_subtract.txt",
			std::minus{});
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>, all_mats<int, 2, 3>>, true>(
			"arithmetic/2x3_subtract.txt",
			std::minus{});
	};

	feature("Multiplication (matrix multiplied with scalar)") = []() {
		test_num_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, false>("arithmetic/2x3_multiply.txt",
			std::multiplies{});
		test_num_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>, true>("arithmetic/2x3_multiply.txt",
			std::multiplies{});
	};

	feature("Multiplication (matrix multiplied with matrix)") = []() {
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 3, 1>, all_mats<int, 2, 1>>, false>(
			"arithmetic/2x3_3x1_multiply.txt",
			std::multiplies{});
		test_op<join_mats<all_mats<int, 2, 3>, all_mats<int, 3, 1>, all_mats<int, 2, 1>>, true>(
			"arithmetic/2x3_3x1_multiply.txt",
			std::multiplies{});
	};

	feature("Division (matrix divided with scalar)") = []() {
		test_num_op<join_mats<all_mats<double, 2, 3>, all_mats<double, 2, 3>>, false>("arithmetic/2x3_divide.txt",
			std::divides{});
		test_num_op<join_mats<all_mats<double, 2, 3>, all_mats<double, 2, 3>>, true>("arithmetic/2x3_divide.txt",
			std::divides{});
	};

	return 0;
}
