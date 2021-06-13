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

// #ifdef _MSC_VER
// #pragma warning(push)
// #pragma warning(disable : 4459)
// #endif

#include <boost/ut.hpp>

// #ifdef _MSC_VER
// #pragma warning(pop)
// #endif

#include <mpp/utility/comparison.hpp>
#include <mpp/utility/type.hpp>
#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_parsers.hpp"

#include <compare>
#include <string_view>
#include <type_traits>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;

template<typename From, typename To, typename... DetArgs>
void test_det(std::string_view filename, const DetArgs&... fn_args)
{
	using ordering_type = std::compare_three_way_result_t<To, To>;

	test(filename.data()) = [=]() {
		const auto [mat, expected_num] = parse_test(filename, parse_mat<From>(), parse_val<To>);

		To num;

		if constexpr (!std::is_same_v<From, To>)
		{
			num = mpp::determinant(std::type_identity<To>{}, mat, fn_args...);
		}
		else
		{
			num = mpp::determinant(mat, fn_args...);
		}

		expect(mpp::floating_point_compare(num, expected_num) == ordering_type::equivalent)
			<< "Output is" << num << "but expected output is" << expected_num;
	};
}

template<typename From, typename To, bool CmpAlloc, typename Fn, typename Alloc, typename... FnArgs>
void test_fn(std::string_view filename,
	const Fn& fn,
	mpp::matrix_type expected_type,
	[[maybe_unused]] const Alloc& alloc_obj,
	const FnArgs&... fn_args)
{
	using from_val_t    = typename From::value_type;
	using to_val_t      = typename To::value_type;
	using ordering_type = std::compare_three_way_result_t<from_val_t, to_val_t>;

	test(filename.data()) = [=, alloc_obj = std::move(alloc_obj)]() {
		const auto [mat, expected_out] = parse_test(filename, parse_mat<From>(), parse_mat<To>());

		auto out = [=, mat = std::move(mat)]() {
			if constexpr (!std::is_same_v<from_val_t, to_val_t>)
			{
				return fn(std::type_identity<to_val_t>{}, mat, fn_args...);
			}
			else
			{
				return fn(mat, fn_args...);
			}
		}();

		expect(mpp::type(out) == expected_type) << "Matrix type doesn't match";
		expect(mpp::elements_compare(out, expected_out) == ordering_type::equivalent) << "Matrices aren't equal";

		if constexpr (CmpAlloc)
		{
			expect(type<typename std::remove_cvref_t<decltype(out)>::allocator_type> == type<Alloc>)
				<< "Type of allocators aren't equal";
			expect(out.get_allocator() == alloc_obj) << "Allocators aren't equal";
		}
	};
}

template<typename MatA, typename MatB, typename MatX, bool CmpAlloc, typename Fn, typename Alloc, typename... FnArgs>
void test_sub(std::string_view filename,
	const Fn& fn,
	mpp::matrix_type expected_type,
	[[maybe_unused]] const Alloc& alloc_obj,
	const FnArgs&... fn_args)
{
	using from_val_t    = std::common_type_t<typename MatA::value_type, typename MatB::value_type>;
	using to_val_t      = typename MatX::value_type;
	using ordering_type = std::compare_three_way_result_t<from_val_t, to_val_t>;

	test(filename.data()) = [=, alloc_obj = std::move(alloc_obj)]() {
		const auto [mat_a, mat_b, expected_out] =
			parse_test(filename, parse_mat<MatA>(), parse_mat<MatB>(), parse_mat<MatX>());

		auto out = [=, mat_a = std::move(mat_a), mat_b = std::move(mat_b)]() {
			if constexpr (!std::is_same_v<from_val_t, to_val_t>)
			{
				return fn(std::type_identity<to_val_t>{}, mat_a, mat_b, fn_args...);
			}
			else
			{
				return fn(mat_a, mat_b, fn_args...);
			}
		}();

		expect(mpp::type(out) == expected_type) << "Matrix type doesn't match";
		expect(mpp::elements_compare(out, expected_out) == ordering_type::equivalent) << "Matrices aren't equal";

		if constexpr (CmpAlloc)
		{
			expect(type<typename std::remove_cvref_t<decltype(out)>::allocator_type> == type<Alloc>)
				<< "Type of allocators aren't equal";
			expect(out.get_allocator() == alloc_obj) << "Allocators aren't equal";
		}
	};
}

template<typename Mat, typename MatL, typename MatU, bool CmpAlloc, typename Alloc, typename... FnArgs>
void test_lu(std::string_view filename,
	mpp::matrix_type expected_type,
	[[maybe_unused]] const Alloc& alloc_obj,
	const FnArgs&... fn_args)
{
	using from_val_t    = typename Mat::value_type;
	using to_val_t      = std::common_type_t<typename MatL::value_type, typename MatU::value_type>;
	using ordering_type = std::compare_three_way_result_t<from_val_t, to_val_t>;

	test(filename.data()) = [=, alloc_obj = std::move(alloc_obj)]() {
		const auto [mat, expected_l, expected_u] =
			parse_test(filename, parse_mat<Mat>(), parse_mat<MatL>(), parse_mat<MatU>());

		auto [out_l, out_u] = [=, mat = std::move(mat)]() {
			if constexpr (!std::is_same_v<from_val_t, to_val_t>)
			{
				return mpp::lu_decomposition(std::type_identity<to_val_t>{}, mat, fn_args...);
			}
			else
			{
				return mpp::lu_decomposition(mat, fn_args...);
			}
		}();

		scenario("Testing L matrix") = [=, expected_l = std::move(expected_l)]() {
			expect(mpp::type(out_l) == expected_type) << "Matrix type doesn't match";
			expect(mpp::elements_compare(out_l, expected_l) == ordering_type::equivalent) << "Matrices aren't equal";

			if constexpr (CmpAlloc)
			{
				expect(type<typename std::remove_cvref_t<decltype(out_l)>::allocator_type> == type<Alloc>)
					<< "Type of allocators aren't equal";
				expect(out_l.get_allocator() == alloc_obj) << "Allocators aren't equal";
			}
		};

		scenario("Testing U matrix") = [=, expected_l = std::move(expected_l)]() {
			expect(mpp::type(out_u) == expected_type) << "Matrix type doesn't match";
			expect(mpp::elements_compare(out_u, expected_u) == ordering_type::equivalent) << "Matrices aren't equal";

			if constexpr (CmpAlloc)
			{
				expect(type<typename std::remove_cvref_t<decltype(out_u)>::allocator_type> == type<Alloc>)
					<< "Type of allocators aren't equal";
				expect(out_u.get_allocator() == alloc_obj) << "Allocators aren't equal";
			}
		};
	};
}

template<typename Mat,
	typename Block,
	bool CmpAlloc,
	typename TopRowIndex,
	typename TopColumnIndex,
	typename BottomRowIndex,
	typename BottomColumnIndex,
	typename Alloc,
	typename... FnArgs>
void test_block(std::string_view filename,
	TopRowIndex top_row_index,
	TopColumnIndex top_column_index,
	BottomRowIndex bottom_row_index,
	BottomColumnIndex bottom_column_index,
	mpp::matrix_type expected_type,
	[[maybe_unused]] const Alloc& alloc_obj,
	const FnArgs&... fn_args)
{
	using from_val_t    = typename Mat::value_type;
	using to_val_t      = typename Block::value_type;
	using ordering_type = std::compare_three_way_result_t<from_val_t, to_val_t>;

	test(filename.data()) = [=, alloc_obj = std::move(alloc_obj)]() {
		const auto [mat, expected_block] = parse_test(filename, parse_mat<Mat>(), parse_mat<Block>());

		auto out = [=, mat = std::move(mat)]() {
			if constexpr (!std::is_same_v<from_val_t, to_val_t>)
			{
				return mpp::block(std::type_identity<to_val_t>{},
					mat,
					top_row_index,
					top_column_index,
					bottom_row_index,
					bottom_column_index,
					fn_args...);
			}
			else
			{
				return mpp::block(mat,
					top_row_index,
					top_column_index,
					bottom_row_index,
					bottom_column_index,
					fn_args...);
			}
		}();

		using out_decayed_t = std::remove_cvref_t<decltype(out)>;

		expect(mpp::type(out) == expected_type) << "Matrix type doesn't match";
		expect(mpp::elements_compare(out, expected_block) == ordering_type::equivalent) << "Matrices aren't equal";

		constexpr auto out_is_dyn =
			out_decayed_t::rows_extent() == mpp::dynamic && out_decayed_t::columns_extent() == mpp::dynamic;

		if constexpr (out_is_dyn && CmpAlloc)
		{
			expect(type<typename out_decayed_t::allocator_type> == type<Alloc>) << "Type of allocators aren't equal";
			expect(out.get_allocator() == alloc_obj) << "Allocators aren't equal";
		}
	};
}

int main()
{
	using namespace mpp;

	using alloc_t       = custom_allocator<double>;
	auto alloc_identity = std::type_identity<alloc_t>{};
	auto alloc_obj      = alloc_t{};

	// @FIXME: Remove tests with `alloc_identity` once #323 has resolved

	feature("Determinant") = [&]() {
		test_det<matrix<int, 0, 0>, int>("algorithm/det/0x0.txt");
		test_det<matrix<int>, int>("algorithm/det/1x1.txt", alloc_identity);
		test_det<matrix<int>, double>("algorithm/det/2x2.txt", alloc_obj);
		test_det<matrix<int>, int>("algorithm/det/3x3.txt", unsafe);
		test_det<matrix<int>, double>("algorithm/det/10x10.txt", unsafe, alloc_identity);
		test_det<matrix<int>, double>("algorithm/det/20x20.txt", unsafe, alloc_obj);
	};

	feature("Transpose") = [&]() { // @FIXME: Test other overloads
		test_fn<matrix<int, 25, 25>, matrix<int, 25, 25>, false>("algorithm/trps/25x25.txt",
			transpose,
			matrix_type::fully_static,
			alloc_obj);
		test_fn<matrix<double>, matrix<double, dynamic, dynamic, custom_allocator<double>>, true>(
			"algorithm/trps/50x2.txt",
			transpose,
			matrix_type::fully_dynamic,
			alloc_obj,
			alloc_identity);
	};

	feature("Inverse") = [&]() {
		test_fn<matrix<double, 0, 0>, matrix<double, 0, 0>, false>("algorithm/inv/0x0.txt",
			inverse,
			matrix_type::fully_static,
			alloc_obj);
		test_fn<matrix<double>, matrix<double, dynamic, dynamic, custom_allocator<double>>, true>(
			"algorithm/inv/1x1.txt",
			inverse,
			matrix_type::fully_dynamic,
			alloc_obj,
			alloc_identity);
		test_fn<matrix<double>, matrix<double, dynamic, dynamic, custom_allocator<double>>, true>(
			"algorithm/inv/2x2.txt",
			inverse,
			matrix_type::fully_dynamic,
			alloc_obj,
			alloc_obj);
		test_fn<matrix<double, 3, 3>, matrix<double, 3, 3>, false>("algorithm/inv/3x3.txt",
			inverse,
			matrix_type::fully_static,
			alloc_obj,
			unsafe);
		test_fn<matrix<int>, matrix<int>, true>("algorithm/inv/3x3_int.txt",
			inverse,
			matrix_type::fully_dynamic,
			alloc_obj,
			unsafe,
			alloc_identity);
		test_fn<matrix<double>, matrix<double>, true>("algorithm/inv/10x10.txt",
			inverse,
			matrix_type::fully_dynamic,
			alloc_obj,
			unsafe,
			alloc_obj);
	};

	feature("Forward substitution") = [&]() { // @FIXME: Test other overloads
		test_sub<matrix<int, 4, 4>, matrix<int, 4, 1>, matrix<int, 4, 1>, false>("algorithm/fwd_sub/4x4_4x1.txt",
			mpp::forward_substitution,
			matrix_type::fully_static,
			alloc_obj);
	};

	feature("Backward substitution") = [&]() { // @FIXME: Test other overloads
		test_sub<matrix<int, 3, 3>, matrix<int, 3, 1>, matrix<int, 3, 1>, false>("algorithm/back_sub/3x3_3x1.txt",
			mpp::back_substitution,
			matrix_type::fully_static,
			alloc_obj);
	};

	feature("LU Decomposition") = [&]() { // @FIXME: Test other overloads
		test_lu<matrix<int, 2, 2>, matrix<int, 2, 2>, matrix<int, 2, 2>, false>("algorithm/lu/2x2.txt",
			matrix_type::fully_static,
			alloc_obj);
		test_lu<matrix<double>, matrix<double>, matrix<double>, true>("algorithm/lu/3x3.txt",
			matrix_type::fully_dynamic,
			alloc_obj,
			alloc_identity);
	};

	feature("Block") = [&]() { // @FIXME: Test other overloads
		test_block<matrix<int, 3, 3>, matrix<int, 1, 1>, false>("algorithm/block/3x3.txt",
			mpp::constant<0>{},
			mpp::constant<0>{},
			mpp::constant<0>{},
			mpp::constant<0>{},
			matrix_type::fully_static,
			alloc_obj);
		test_block<matrix<double>, matrix<double>, false>("algorithm/block/4x4.txt",
			3,
			2,
			3,
			2,
			matrix_type::fully_dynamic,
			alloc_obj,
			alloc_identity);
	};

	return 0;
}
