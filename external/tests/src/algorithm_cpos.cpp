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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4459)
#endif

#include <boost/ut.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <mpp/utility/comparison.hpp>
#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_parsers.hpp"

#include <compare>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;

template<typename From, typename To>
void test_det(const std::string& filename)
{
	const auto result       = parse_num_out<From, To>(get_filepath(filename));
	const auto& mat         = result.mat;
	const auto expected_num = result.num;

	using ordering_type = std::compare_three_way_result_t<To, To>;
	using cust_alloc_t  = custom_allocator<mpp::detail::default_floating_type>;

	const auto do_cmp = [&](const auto&... args) {
		To num;

		if constexpr (!std::is_same_v<From, To>)
		{
			num = mpp::determinant(std::type_identity<To>{}, mat, args...);
		}
		else
		{
			num = mpp::determinant(mat, args...);
		}

		expect(mpp::floating_point_compare(num, expected_num) == ordering_type::equivalent);
	};

	test(filename) = [&]() {
		const auto cust_alloc = cust_alloc_t{};

		when("Not using unsafe") = [&]() {
			do_cmp();
			do_cmp(std::type_identity<cust_alloc_t>{});
			do_cmp(cust_alloc);
		};

		when("Using unsafe") = [&]() {
			do_cmp(mpp::unsafe);
			do_cmp(mpp::unsafe, std::type_identity<cust_alloc_t>{});
			do_cmp(mpp::unsafe, cust_alloc);
		};
	};
}

template<typename From, typename To, typename CustAlloc, bool HasUnsafeOverload, bool PassToAsIdentityObj>
void test_transformation(const std::string& filename, const auto& transform_fn)
{
	const auto result =
		parse_mats_out<temp_types<mat_t, mat_t>, types<From, To>>(get_filepath(filename), std::tuple{ mat_fn, mat_fn });
	const auto& mat          = std::get<0>(result);
	const auto& expected_out = std::get<1>(result);

	using ordering_type = std::compare_three_way_result_t<To, To>;

	const auto do_cmp = [&](const auto&... args) {
		const auto out = [&]() {
			if constexpr (PassToAsIdentityObj)
			{
				return transform_fn(std::type_identity<To>{}, mat, args...);
			}
			else
			{
				return transform_fn(mat, args...);
			}
		}();

		expect(out.rows() == expected_out.rows());
		expect(out.columns() == expected_out.columns());

		expect(mpp::elements_compare(out, expected_out, mpp::floating_point_compare) == ordering_type::equivalent);
	};

	test(filename) = [&]() {
		const auto cust_alloc = CustAlloc{};

		when("Not using unsafe") = [&]() {
			do_cmp();
			do_cmp(std::type_identity<CustAlloc>{});
			do_cmp(cust_alloc);
		};

		if constexpr (HasUnsafeOverload)
		{
			when("Using unsafe") = [&]() {
				do_cmp(mpp::unsafe);
				do_cmp(mpp::unsafe, std::type_identity<CustAlloc>{});
				do_cmp(mpp::unsafe, cust_alloc);
			};
		}
	};
}

template<typename From, typename To, typename CustAlloc>
void test_block(const std::string& filename)
{
	const auto result  = parse_block_out<From, To>(get_filepath(filename));
	const auto& mat    = result.mat;
	const auto& blocks = result.blocks;

	using ordering_type = std::compare_three_way_result_t<To, To>;

	const auto do_cmp = [&](const auto& expected_block,
							std::size_t top_row_index,
							std::size_t top_column_index,
							std::size_t bottom_row_index,
							std::size_t bottom_column_index,
							const auto&... args) {
		const auto block =
			mpp::block(mat, top_row_index, top_column_index, bottom_row_index, bottom_column_index, args...);

		expect(block.rows() == expected_block.rows());
		expect(block.columns() == expected_block.columns());

		expect(mpp::elements_compare(block, expected_block, mpp::floating_point_compare) == ordering_type::equivalent);
	};

	test(filename) = [&]() {
		const auto cust_alloc = CustAlloc{};

		for (const auto& block : blocks)
		{
			const auto row_start       = block.row_start;
			const auto column_start    = block.column_start;
			const auto row_end         = block.row_end;
			const auto column_end      = block.column_end;
			const auto block_          = mpp::block(mat, row_start, column_start, row_end, column_end);
			const auto& expected_block = block.mat;

			when("Not using unsafe") = [&]() {
				do_cmp(expected_block, row_start, column_start, row_end, column_end);
				do_cmp(expected_block, row_start, column_start, row_end, column_end, std::type_identity<CustAlloc>{});
				do_cmp(expected_block, row_start, column_start, row_end, column_end, cust_alloc);
			};

			when("Using unsafe") = [&]() {
				do_cmp(expected_block, row_start, column_start, row_end, column_end, mpp::unsafe);
				do_cmp(expected_block,
					row_start,
					column_start,
					row_end,
					column_end,
					mpp::unsafe,
					std::type_identity<CustAlloc>{});
				do_cmp(expected_block, row_start, column_start, row_end, column_end, mpp::unsafe, cust_alloc);
			};
		}
	};
}

template<typename From, typename To, typename CustAlloc, bool PassToAsIdentityObj>
void test_lu(const std::string& filename)
{
	const auto result = parse_mats_out<temp_types<mat_t, mat_t, mat_t>, types<From, To, To>>(get_filepath(filename),
		std::tuple{ mat_fn, mat_fn, mat_fn });
	const auto& mat   = std::get<0>(result);
	const auto& expected_left  = std::get<1>(result);
	const auto& expected_right = std::get<2>(result);

	using ordering_type = std::compare_three_way_result_t<To, To>;

	const auto do_cmp = [&](const auto&... args) {
		const auto result = [&]() {
			if constexpr (PassToAsIdentityObj)
			{
				return mpp::lu_decomposition(std::type_identity<To>{}, mat, args...);
			}
			else
			{
				return mpp::lu_decomposition(mat, args...);
			}
		}();

		const auto& left  = result.first;
		const auto& right = result.second;

		given("L matrix") = [&]() {
			expect(left.rows() == expected_left.rows());
			expect(left.columns() == expected_left.columns());

			expect(
				mpp::elements_compare(left, expected_left, mpp::floating_point_compare) == ordering_type::equivalent);
		};

		given("U matrix") = [&]() {
			expect(right.rows() == expected_right.rows());
			expect(right.columns() == expected_right.columns());

			expect(
				mpp::elements_compare(right, expected_right, mpp::floating_point_compare) == ordering_type::equivalent);
		};
	};

	test(filename) = [&]() {
		const auto cust_alloc = CustAlloc{};

		when("Not using unsafe") = [&]() {
			do_cmp();
			do_cmp(std::type_identity<CustAlloc>{});
			do_cmp(cust_alloc);
		};

		when("Using unsafe") = [&]() {
			do_cmp(mpp::unsafe);
			do_cmp(mpp::unsafe, std::type_identity<CustAlloc>{});
			do_cmp(mpp::unsafe, cust_alloc);
		};
	};
}

template<typename AValue, typename BValue, typename XValue, typename CustAlloc, bool PassToAsIdentityObj>
void test_sub(const std::string& filename, const auto& fn)
{
	const auto result =
		parse_mats_out<temp_types<mat_t, mat_t, mat_t>, types<AValue, BValue, XValue>>(get_filepath(filename),
			std::tuple{ mat_fn, mat_fn, mat_fn });
	const auto& a          = std::get<0>(result);
	const auto& b          = std::get<1>(result);
	const auto& expected_x = std::get<2>(result);

	using ordering_type = std::compare_three_way_result_t<XValue, XValue>;

	const auto do_cmp = [&](const auto&... args) {
		const auto x = [&]() {
			if constexpr (PassToAsIdentityObj)
			{
				return fn(std::type_identity<XValue>{}, a, b, args...);
			}
			else
			{
				return fn(a, b, args...);
			}
		}();

		expect(x.rows() == expected_x.rows());
		expect(x.columns() == expected_x.columns());

		expect(mpp::elements_compare(x, expected_x, mpp::floating_point_compare) == ordering_type::equivalent);
	};

	test(filename) = [&]() {
		const auto cust_alloc = CustAlloc{};

		when("Not using unsafe") = [&]() {
			do_cmp();
			do_cmp(std::type_identity<CustAlloc>{});
			do_cmp(cust_alloc);
		};

		when("Using unsafe") = [&]() {
			do_cmp(mpp::unsafe);
			do_cmp(mpp::unsafe, std::type_identity<CustAlloc>{});
			do_cmp(mpp::unsafe, cust_alloc);
		};
	};
}

int main()
{
	feature("Determinant") = []() {
		test_det<int, int>("algorithm/det/0x0.txt");
		test_det<int, int>("algorithm/det/1x1.txt");
		test_det<int, double>("algorithm/det/2x2.txt"); // @NOTE: MSVC with stol would be out of range
		test_det<int, int>("algorithm/det/3x3.txt");
		test_det<int, double>("algorithm/det/10x10.txt");
	};

	feature("Transpose") = []() {
		test_transformation<int, int, custom_allocator<int>, false, false>("algorithm/t/25x25.txt", mpp::transpose);
		test_transformation<int, int, custom_allocator<int>, false, false>("algorithm/t/50x2.txt", mpp::transpose);
	};

	feature("LU Decomposition") = []() {
		test_lu<int, double, custom_allocator<double>, true>("algorithm/lu/2x2.txt");
		test_lu<int, double, custom_allocator<double>, true>("algorithm/lu/3x3.txt");
	};

	feature("Inverse") = []() {
		test_transformation<int, double, custom_allocator<double>, true, true>("algorithm/inv/2x2.txt", mpp::inverse);
		test_transformation<int, double, custom_allocator<double>, true, true>("algorithm/inv/3x3.txt", mpp::inverse);
		test_transformation<int, double, custom_allocator<double>, true, true>("algorithm/inv/3x3_int.txt",
			mpp::inverse);
		test_transformation<double, double, custom_allocator<double>, true, true>("algorithm/inv/10x10.txt",
			mpp::inverse);
	};

	feature("Block") = []() {
		test_block<int, int, custom_allocator<int>>("algorithm/block/4x4.txt");
	};

	feature("Forward substitution") = []() {
		// @TODO: Add testcase where a, b, and x matrix have different value type to test with PassToAsIdentityObj =
		// true

		test_sub<int, int, int, custom_allocator<int>, false>("algorithm/fwd_sub/4x4_4x1.txt",
			mpp::forward_substitution);
	};

	feature("Back substitution") = []() {
		// @TODO: Add testcase where a, b, and x matrix have same value type to test with PassToAsIdentityObj = false

		test_sub<int, int, float, custom_allocator<float>, true>("algorithm/back_sub/3x3_3x1.txt",
			mpp::back_substitution);
	};

	return 0;
}
