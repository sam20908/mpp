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

#include <mpp/matrix.hpp>

#include "../../include/test_utilities.hpp"

#include <initializer_list>
#include <vector>

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace mpp;

namespace
{
	template<typename Mats, bool Move, bool UseArr2DAsRng>
	void test_assign_rng(std::string_view test_name)
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

			auto [mat, vec2d, expected_mat] = [&]() {
				if constexpr (UseArr2DAsRng)
				{
					return parse_test(test_name,
						parse_mat<mat_t>,
						parse_arr2d<T, RowsExtent, ColumnsExtent>,
						parse_mat<mat2_t>);
				}
				else
				{
					return parse_test(test_name, parse_mat<mat_t>, parse_vec2d<T>, parse_mat<mat2_t>);
				}
			}();

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
		test(test_name.data()) = [&, test_name]<typename T,
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

			auto [mat, mat2, expected_mat] =
				parse_test(test_name, parse_mat<mat_t>, parse_mat<mat2_t>, parse_mat<mat3_t>);

			if constexpr (Move)
			{
				mat = std::move(mat2);
			}
			else
			{
				mat = mat2;
			}

			cmp_mat_to_expr_like(mat, expected_mat);
		} | Mats{};
	}
} // namespace


int main()
{
	// @NOTE: This covers copy and move assignment from rule of five (2/5)

	feature("Assigning a 2D range with same dimensions") = []() {
		using mats = join_mats<all_mats<int, 2, 3>, all_mats<int, 2, 3>>;

		test_assign_rng<mats, false, false>("assignment/2x3_same_dims.txt");
		test_assign_rng<mats, true, false>("assignment/2x3_same_dims.txt");
	};

	feature("Assigning a 2D array with same dimensions (fixed matrices only)") = []() {
		using mats = join_mats<fixed_mat<int, 2, 3>, fixed_mat<int, 2, 3>>;

		test_assign_rng<mats, false, true>("assignment/2x3_same_dims.txt");
		test_assign_rng<mats, true, true>("assignment/2x3_same_dims.txt");
	};

	feature("Expanding dynamic matrices by range (dynamic matrices only)") = []() {
		test_assign_rng<join_mats<dyn_mat<int>, dyn_mat<int>>, true, false>("assignment/2x3_10x10_expand_dyn_mat.txt");
		test_assign_rng<join_mats<dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>>, true, false>(
			"assignment/2x3_10x3_expand_dyn_rows_mat.txt");
		test_assign_rng<join_mats<dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>>, true, false>(
			"assignment/2x3_2x10_expand_dyn_cols_mat.txt");
	};

	feature("Shrinking dynamic matrices by range (dynamic matrices only)") = []() {
		test_assign_rng<join_mats<dyn_mat<int>, dyn_mat<int>>, true, false>("assignment/10x10_2x3_shrink_dyn_mat.txt");
		test_assign_rng<join_mats<dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>>, true, false>(
			"assignment/10x3_2x3_shrink_dyn_rows_mat.txt");
		test_assign_rng<join_mats<dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>>, true, false>(
			"assignment/2x10_2x3_shrink_dyn_cols_mat.txt");
	};

	feature("Expanding dynamic matrices by matrix (dynamic matrices only)") = []() {
		test_assign_mat<join_mats<dyn_mat<int>, dyn_mat<int>, dyn_mat<int>>, true>(
			"assignment/2x3_10x10_expand_dyn_mat.txt");
		test_assign_mat<join_mats<dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>>, true>(
			"assignment/2x3_10x3_expand_dyn_rows_mat.txt");
		test_assign_mat<join_mats<dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>>, true>(
			"assignment/2x3_2x10_expand_dyn_cols_mat.txt");
	};

	feature("Shrinking dynamic matrices by matrix (dynamic matrices only)") = []() {
		test_assign_mat<join_mats<dyn_mat<int>, dyn_mat<int>, dyn_mat<int>>, true>(
			"assignment/10x10_2x3_shrink_dyn_mat.txt");
		test_assign_mat<join_mats<dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>, dyn_rows_mat<int, 3>>, true>(
			"assignment/10x3_2x3_shrink_dyn_rows_mat.txt");
		test_assign_mat<join_mats<dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>, dyn_cols_mat<int, 2>>, true>(
			"assignment/2x10_2x3_shrink_dyn_cols_mat.txt");
	};

	return 0;
}
