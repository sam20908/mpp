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

#include "../../include/test_parsers.hpp"

#include <compare>
#include <filesystem>
#include <functional>
#include <string>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;

static auto get_filepath(const std::string& filename) -> std::filesystem::path
{
	return std::filesystem::path(TEST_DATA_PATH) / filename;
}

template<typename From, typename To>
void test_det(const std::string& filename)
{
	const auto result       = parse_num_out<From, To>(get_filepath(filename));
	const auto& mat         = result.mat;
	const auto expected_num = result.num;

	test(filename) = [&]() {
		const auto num = mpp::determinant(std::type_identity<To>{}, mat);

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::floating_point_compare(num, expected_num) == ordering_type::equivalent);
	};
}

template<typename From, typename To>
void test_transformation(const std::string& filename, const auto& transform_fn)
{
	const auto result        = parse_mat_out<From, To>(get_filepath(filename));
	const auto& mat          = result.mat;
	const auto& expected_out = result.out;

	test(filename) = [&]() {
		const auto out = transform_fn(mat);

		expect(out.rows() == expected_out.rows());
		expect(out.columns() == expected_out.columns());

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::elements_compare(out, expected_out, mpp::floating_point_compare) == ordering_type::equivalent);
	};
}

template<typename From, typename To>
void test_block(const std::string& filename)
{
	const auto result  = parse_block_out<From, To>(get_filepath(filename));
	const auto& mat    = result.mat;
	const auto& blocks = result.blocks;

	test(filename) = [&]() {
		using ordering_type = std::compare_three_way_result_t<To, To>;

		for (const auto& block : blocks)
		{
			const auto row_start       = block.row_start;
			const auto column_start    = block.column_start;
			const auto row_end         = block.row_end;
			const auto column_end      = block.column_end;
			const auto block_          = mpp::block(mat, row_start, column_start, row_end, column_end);
			const auto& expected_block = block.mat;

			expect(block_.rows() == expected_block.rows());
			expect(block_.columns() == expected_block.columns());

			expect(mpp::elements_compare(block_, expected_block, mpp::floating_point_compare) ==
				   ordering_type::equivalent);
		}
	};
}

template<typename From, typename To>
void test_lu(const std::string& filename)
{
	const auto result          = parse_two_mat_out<From, To>(get_filepath(filename));
	const auto& mat            = result.mat;
	const auto& expected_left  = result.left;
	const auto& expected_right = result.right;

	test(filename) = [&]() {
		const auto [left, right] = mpp::lu_decomposition(std::type_identity<To>{}, mat);

		expect(left.rows() == expected_left.rows());
		expect(left.columns() == expected_left.columns());
		expect(right.rows() == expected_right.rows());
		expect(right.columns() == expected_right.columns());

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::elements_compare(left, expected_left, mpp::floating_point_compare) == ordering_type::equivalent);
		expect(mpp::elements_compare(right, expected_right, mpp::floating_point_compare) == ordering_type::equivalent);
	};
}

template<typename AValue, typename BValue, typename XValue>
void test_sub(const std::string& filename, const auto& fn)
{
	const auto result      = parse_subst_out<AValue, BValue, XValue>(get_filepath(filename));
	const auto& a          = result.a;
	const auto& b          = result.b;
	const auto& expected_x = result.x;

	test(filename) = [&]() {
		const auto x = fn(a, b);

		expect(x.rows() == expected_x.rows());
		expect(x.columns() == expected_x.columns());

		using ordering_type = std::compare_three_way_result_t<XValue, XValue>;

		expect(mpp::elements_compare(x, expected_x, mpp::floating_point_compare) == ordering_type::equivalent);
	};
}

int main()
{
	feature("Determinant") = []() {
		test_det<int, int>("test_data/algorithm/det/0x0.txt");
		test_det<int, int>("test_data/algorithm/det/1x1.txt");
		test_det<int, long>("test_data/algorithm/det/2x2.txt");
		test_det<int, int>("test_data/algorithm/det/3x3.txt");
		test_det<int, double>("test_data/algorithm/det/10x10.txt");
	};

	feature("Transpose") = []() {
		test_transformation<int, int>("test_data/algorithm/t/25x25.txt", mpp::transpose);
		test_transformation<int, int>("test_data/algorithm/t/50x2.txt", mpp::transpose);
	};

	feature("LU Decomposition") = []() {
		test_lu<int, double>("test_data/algorithm/lu/2x2.txt");
		test_lu<int, double>("test_data/algorithm/lu/3x3.txt");
	};

	feature("Inverse") = []() {
		auto inv_fn = std::bind_front(mpp::inverse, std::type_identity<double>{});

		test_transformation<int, double>("test_data/algorithm/inv/2x2.txt", inv_fn);
		test_transformation<int, double>("test_data/algorithm/inv/3x3.txt", inv_fn);
		test_transformation<int, double>("test_data/algorithm/inv/3x3_int.txt", inv_fn);
		test_transformation<double, double>("test_data/algorithm/inv/10x10.txt", inv_fn);
	};

	feature("Block") = []() {
		test_block<int, int>("test_data/algorithm/block/4x4.txt");
	};

	feature("Forward substitution") = []() {
		auto fwd_sub_fn = std::bind_front(mpp::forward_substitution, std::type_identity<int>{});

		test_sub<int, int, int>("test_data/algorithm/fwd_sub/4x4_4x1.txt", fwd_sub_fn);
	};

	feature("Back substitution") = []() {
		auto back_sub_fn = std::bind_front(mpp::back_substitution, std::type_identity<double>{});

		test_sub<int, int, double>("test_data/algorithm/back_sub/3x3_3x1.txt", back_sub_fn);
	};

	return 0;
}
