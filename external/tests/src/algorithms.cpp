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

#include <mpp/utility/comparison.hpp>
#include <mpp/utility/type.hpp>
#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_utilities.hpp"

#include <compare>
#include <string_view>
#include <type_traits>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

namespace
{
	template<typename Mats, typename From, typename To>
	void test_det(std::string_view test_name, const auto&... fn_args)
	{
		test(test_name.data()) =
			[&, test_name]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
				std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>) {
				using mat_t = matrix<T, RowsExtent, ColumnsExtent, Alloc>;

				const auto [mat, expected_num] = parse_test(test_name, parse_mat<mat_t>, parse_val<To>);

				log_mat(mat);

				auto num = [&]() {
					if constexpr (!std::is_same_v<From, To>)
					{
						return mpp::determinant(std::type_identity<To>{}, mat, fn_args...);
					}
					else
					{
						return mpp::determinant(mat, fn_args...);
					}
				}();

				cmp_nums(num, expected_num);
			} |
			Mats{};
	}

	template<typename Mats>
	void test_fn(std::string_view test_name, const auto& fn, const auto&... fn_args)
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

			const auto [mat, expected_mat] = parse_test(test_name, parse_mat<mat_t>, parse_mat<mat2_t>);

			const auto out = [&]() {
				if constexpr (!std::is_same_v<T, T2>)
				{
					return fn(std::type_identity<T2>{}, mat, fn_args...);
				}
				else
				{
					return fn(mat, fn_args...);
				}
			}();

			log_mat(mat);

			cmp_mat_types(out, expected_mat);
			cmp_mats(out, expected_mat);
		} | Mats{};
	}

	template<typename Mats>
	void test_sub(std::string_view test_name, const auto& fn, const auto&... fn_args)
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
			using mat_t      = matrix<T, RowsExtent, ColumnsExtent, Alloc>;
			using mat2_t     = matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>;
			using mat3_t     = matrix<T3, RowsExtent3, ColumnsExtent3, Alloc3>;
			using from_val_t = std::common_type_t<T, T2>;

			const auto [mat_a, mat_b, expected_mat] =
				parse_test(test_name, parse_mat<mat_t>, parse_mat<mat2_t>, parse_mat<mat3_t>);

			log_mats2(mat_a, mat_b);

			const auto out = [&]() {
				if constexpr (!std::is_same_v<from_val_t, T3>)
				{
					return fn(std::type_identity<T3>{}, mat_a, mat_b, fn_args...);
				}
				else
				{
					return fn(mat_a, mat_b, fn_args...);
				}
			}();

			cmp_mat_types(out, expected_mat);
			cmp_mats(out, expected_mat);
		} | Mats{};
	}

	template<typename Mats>
	void test_lu(std::string_view test_name, const auto&... fn_args)
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

			const auto [mat, expected_l, expected_u] =
				parse_test(test_name, parse_mat<mat_t>, parse_mat<mat2_t>, parse_mat<mat3_t>);

			log_mat(mat);

			const auto [out_l, out_u] = [&]() {
				if constexpr (!std::is_same_v<T, T2>)
				{
					return mpp::lu_decomposition(std::type_identity<T2>{}, mat, fn_args...);
				}
				else
				{
					return mpp::lu_decomposition(mat, fn_args...);
				}
			}();

			scenario("Testing L matrix") = [&]() {
				cmp_mat_types(out_l, expected_l);
				cmp_mats(out_l, expected_l);
			};

			scenario("Testing U matrix") = [&]() {
				cmp_mat_types(out_u, expected_u);
				cmp_mats(out_u, expected_u);
			};
		} | Mats{};
	}
} // namespace

// template<typename Mat,
// 	typename Block,
// 	bool CmpAlloc,
// 	typename TopRowIndex,
// 	typename TopColumnIndex,
// 	typename BottomRowIndex,
// 	typename BottomColumnIndex,
// 	typename Alloc,
// 	typename... FnArgs>
// void test_block(std::string_view filename,
// 	TopRowIndex top_row_index,
// 	TopColumnIndex top_column_index,
// 	BottomRowIndex bottom_row_index,
// 	BottomColumnIndex bottom_column_index,
// 	mpp::matrix_type expected_type,
// 	[[maybe_unused]] const Alloc& alloc_obj,
// 	const FnArgs&... fn_args)
// {
// 	using from_val_t    = typename Mat::value_type;
// 	using to_val_t      = typename Block::value_type;
// 	using ordering_type = std::compare_three_way_result_t<from_val_t, to_val_t>;

// 	test(filename.data()) = [=, alloc_obj = std::move(alloc_obj)]() {
// 		const auto [mat, expected_block] = parse_test(filename, parse_mat<Mat>(), parse_mat<Block>());

// 		auto out = [=, mat = std::move(mat)]() {
// 			if constexpr (!std::is_same_v<from_val_t, to_val_t>)
// 			{
// 				return mpp::block(std::type_identity<to_val_t>{},
// 					mat,
// 					top_row_index,
// 					top_column_index,
// 					bottom_row_index,
// 					bottom_column_index,
// 					fn_args...);
// 			}
// 			else
// 			{
// 				return mpp::block(mat,
// 					top_row_index,
// 					top_column_index,
// 					bottom_row_index,
// 					bottom_column_index,
// 					fn_args...);
// 			}
// 		}();

// 		expect(mpp::type(out) == expected_type) << "Matrix type doesn't match";
// 		expect(mpp::elements_compare(out, expected_block) == ordering_type::equivalent) << "Matrices aren't equal";

// 		if constexpr (CmpAlloc)
// 		{
// 			expect(out.get_allocator() == alloc_obj) << "Allocators aren't equal";
// 		}
// 	};
// }

int main()
{
	using namespace mpp;

	using alloc_t             = custom_allocator<double>;
	const auto alloc_identity = std::type_identity<alloc_t>{};
	const auto alloc_obj      = alloc_t{};

	// @FIXME: Remove tests with `alloc_identity` once #323 has resolved

	feature("Determinant") = [&]() {
		test_det<all_mats<int, 0, 0>, int, int>("algorithm/det/0x0.txt");
		test_det<all_mats<double, 0, 0, alloc_t>, int, int>("algorithm/det/0x0.txt");
		test_det<all_mats<double, 0, 0, alloc_t>, int, int>("algorithm/det/0x0.txt", alloc_identity);
		test_det<all_mats<double, 0, 0, alloc_t>, int, int>("algorithm/det/0x0.txt", alloc_obj);
		test_det<all_mats<double, 0, 0, alloc_t>, int, int>("algorithm/det/0x0.txt", unsafe, alloc_identity);
		test_det<all_mats<double, 0, 0, alloc_t>, int, int>("algorithm/det/0x0.txt", unsafe, alloc_obj);
	};

	feature("Transpose") = [&]() {
		test_fn<join_mats_t<all_mats<int, 25, 25>, all_trps_mats<int, 25, 25>>>("algorithm/trps/25x25.txt", transpose);
		test_fn<join_mats_t<dyn_mats<double, 50, 2, alloc_t>, dyn_trps_mats<double, 50, 2, alloc_t>>>(
			"algorithm/trps/50x2.txt",
			transpose);
		test_fn<join_mats_t<dyn_mats<double, 50, 2>, dyn_trps_mats<double, 50, 2, alloc_t>>>("algorithm/trps/50x2.txt",
			transpose,
			alloc_identity);
		test_fn<join_mats_t<dyn_mats<double, 50, 2>, dyn_trps_mats<double, 50, 2, alloc_t>>>("algorithm/trps/50x2.txt",
			transpose,
			alloc_obj);
	};

	feature("Inverse") = [&]() {
		test_fn<join_mats_t<all_mats<double, 0, 0>, all_mats<double, 0, 0>>>("algorithm/inv/0x0.txt", inverse);
		test_fn<join_mats_t<all_mats<int, 3, 3>, all_mats<double, 3, 3>>>("algorithm/inv/3x3_int.txt", inverse);
		test_fn<join_mats_t<dyn_mats<double, 1, 1, alloc_t>, dyn_mats<double, 1, 1, alloc_t>>>("algorithm/inv/1x1.txt",
			inverse);
		test_fn<join_mats_t<dyn_mats<double, 2, 2>, dyn_mats<double, 2, 2, alloc_t>>>("algorithm/inv/2x2.txt",
			inverse,
			alloc_identity);
		test_fn<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 3, alloc_t>>>("algorithm/inv/3x3.txt",
			inverse,
			alloc_obj);
		test_fn<join_mats_t<dyn_mats<double, 10, 10>, dyn_mats<double, 10, 10, alloc_t>>>("algorithm/inv/10x10.txt",
			inverse,
			unsafe,
			alloc_identity);
		test_fn<join_mats_t<dyn_mats<double, 10, 10>, dyn_mats<double, 10, 10, alloc_t>>>("algorithm/inv/10x10.txt",
			inverse,
			unsafe,
			alloc_obj);
	};

	feature("Forward substitution") = [&]() {
		test_sub<join_mats_t<all_mats<int, 4, 4>, all_mats<int, 4, 1>, all_sub_mats<int, 4, 1, 1>>>(
			"algorithm/fwd_sub/4x4_4x1.txt",
			forward_substitution);
		test_sub<join_mats_t<dyn_mats<double, 4, 4, alloc_t>,
			dyn_mats<double, 4, 1, alloc_t>,
			dyn_sub_mats<double, 4, 1, 1, alloc_t>>>("algorithm/fwd_sub/4x4_4x1.txt", forward_substitution);
		test_sub<join_mats_t<dyn_mats<double, 4, 4>, dyn_mats<double, 4, 1>, dyn_sub_mats<double, 4, 1, 1, alloc_t>>>(
			"algorithm/fwd_sub/4x4_4x1.txt",
			forward_substitution,
			alloc_identity);
		test_sub<join_mats_t<dyn_mats<double, 4, 4>, dyn_mats<double, 4, 1>, dyn_sub_mats<double, 4, 1, 1, alloc_t>>>(
			"algorithm/fwd_sub/4x4_4x1.txt",
			forward_substitution,
			alloc_obj);
		test_sub<join_mats_t<dyn_mats<double, 4, 4>, dyn_mats<double, 4, 1>, dyn_sub_mats<double, 4, 1, 1, alloc_t>>>(
			"algorithm/fwd_sub/4x4_4x1.txt",
			forward_substitution,
			unsafe,
			alloc_identity);
		test_sub<join_mats_t<dyn_mats<double, 4, 4>, dyn_mats<double, 4, 1>, dyn_sub_mats<double, 4, 1, 1, alloc_t>>>(
			"algorithm/fwd_sub/4x4_4x1.txt",
			forward_substitution,
			unsafe,
			alloc_obj);
	};

	feature("Backward substitution") = [&]() {
		test_sub<join_mats_t<all_mats<int, 3, 3>, all_mats<int, 3, 1>, all_sub_mats<int, 3, 1, 1>>>(
			"algorithm/back_sub/3x3_3x1.txt",
			back_substitution);
		test_sub<join_mats_t<dyn_mats<double, 3, 3, alloc_t>,
			dyn_mats<double, 3, 1, alloc_t>,
			dyn_sub_mats<double, 3, 1, 1, alloc_t>>>("algorithm/back_sub/3x3_3x1.txt", back_substitution);
		test_sub<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 1>, dyn_sub_mats<double, 3, 1, 1, alloc_t>>>(
			"algorithm/back_sub/3x3_3x1.txt",
			back_substitution,
			alloc_identity);
		test_sub<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 1>, dyn_sub_mats<double, 3, 1, 1, alloc_t>>>(
			"algorithm/back_sub/3x3_3x1.txt",
			back_substitution,
			alloc_obj);
		test_sub<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 1>, dyn_sub_mats<double, 3, 1, 1, alloc_t>>>(
			"algorithm/back_sub/3x3_3x1.txt",
			back_substitution,
			unsafe,
			alloc_identity);
		test_sub<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 1>, dyn_sub_mats<double, 3, 1, 1, alloc_t>>>(
			"algorithm/back_sub/3x3_3x1.txt",
			back_substitution,
			unsafe,
			alloc_obj);
	};

	feature("LU Decomposition") = [&]() {
		test_lu<join_mats_t<all_mats<int, 2, 2>, all_mats<int, 2, 2>, all_mats<int, 2, 2>>>("algorithm/lu/2x2.txt");
		test_lu<join_mats_t<dyn_mats<double, 3, 3, alloc_t>,
			dyn_mats<double, 3, 3, alloc_t>,
			dyn_mats<double, 3, 3, alloc_t>>>("algorithm/lu/3x3.txt");
		test_lu<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 3, alloc_t>, dyn_mats<double, 3, 3, alloc_t>>>(
			"algorithm/lu/3x3.txt",
			alloc_identity);
		test_lu<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 3, alloc_t>, dyn_mats<double, 3, 3, alloc_t>>>(
			"algorithm/lu/3x3.txt",
			alloc_obj);
		test_lu<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 3, alloc_t>, dyn_mats<double, 3, 3, alloc_t>>>(
			"algorithm/lu/3x3.txt",
			unsafe,
			alloc_identity);
		test_lu<join_mats_t<dyn_mats<double, 3, 3>, dyn_mats<double, 3, 3, alloc_t>, dyn_mats<double, 3, 3, alloc_t>>>(
			"algorithm/lu/3x3.txt",
			unsafe,
			alloc_obj);
	};

	// feature("Block") = [&]() { // @FIXME: Test other overloads
	// 	test_block<matrix<int, 3, 3>, matrix<int, 1, 1>, false>("algorithm/block/3x3.txt",
	// 		mpp::constant<0>{},
	// 		mpp::constant<0>{},
	// 		mpp::constant<0>{},
	// 		mpp::constant<0>{},
	// 		matrix_type::fully_static,
	// 		alloc_obj);
	// 	test_block<matrix<double>, matrix<double>, false>("algorithm/block/4x4.txt",
	// 		3,
	// 		2,
	// 		3,
	// 		2,
	// 		matrix_type::fully_dynamic,
	// 		alloc_obj,
	// 		alloc_identity);
	// };

	return 0;
}
