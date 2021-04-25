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

#include "../../include/data_parser.hpp"
#include "../../include/utility.hpp"

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
void test_determinant(const std::string& filename)
{
	const auto result_struct  = parse_data_file_with_single_number_result<From, To>(get_filepath(filename));
	const auto& original_data = result_struct.original_data;
	const auto result         = result_struct.result;

	test(filename) = [&]() {
		const auto det = mpp::determinant(std::type_identity<To>{}, original_data);

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::floating_point_compare(det, result) == ordering_type::equivalent);
	};
}

template<typename From, typename To>
void test_transformation(const std::string& filename, const auto& transform_fn)
{
	const auto result_struct       = parse_data_file_matrix_transformation<From, To>(get_filepath(filename));
	const auto& original_matrix    = result_struct.original_matrix;
	const auto& transformed_matrix = result_struct.transformed_matrix;

	test(filename) = [&]() {
		const auto result_matrix = transform_fn(original_matrix);

		expect(result_matrix.rows() == transformed_matrix.rows());
		expect(result_matrix.columns() == transformed_matrix.columns());

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::elements_compare(result_matrix, transformed_matrix, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
	};
}

template<typename From, typename To>
void test_block(const std::string& filename)
{
	const auto result_struct    = parse_data_file_block_transformation<From, To>(get_filepath(filename));
	const auto& original_matrix = result_struct.original_matrix;
	const auto& blocks          = result_struct.blocks;

	test(filename) = [&]() {
		for (const auto& block : blocks)
		{
			const auto row_start    = block.row_start;
			const auto column_start = block.column_start;
			const auto row_end      = block.row_end;
			const auto column_end   = block.column_end;
			const auto cropped_original_matrix =
				mpp::block(original_matrix, row_start, column_start, row_end, column_end);
			const auto& block_matrix = block.block_matrix;

			expect(cropped_original_matrix.rows() == block_matrix.rows());
			expect(cropped_original_matrix.columns() == block_matrix.columns());

			using ordering_type = std::compare_three_way_result_t<To, To>;

			expect(mpp::elements_compare(cropped_original_matrix, block_matrix, mpp::floating_point_compare) ==
				   ordering_type::equivalent);
		}
	};
}

template<typename From, typename To>
void test_lu_decomposition(const std::string& filename)
{
	const auto result_struct      = parse_data_file_two_matrices_transformation<From, To>(get_filepath(filename));
	const auto& original_matrix   = result_struct.original_matrix;
	const auto& l_expected_matrix = result_struct.l_matrix;
	const auto& u_expected_matrix = result_struct.u_matrix;

	test(filename) = [&]() {
		const auto [l_matrix, u_matrix] = mpp::lu_decomposition(std::type_identity<To>{}, original_matrix);

		expect(l_matrix.rows() == l_expected_matrix.rows());
		expect(l_matrix.columns() == l_expected_matrix.columns());
		expect(u_matrix.rows() == u_expected_matrix.rows());
		expect(u_matrix.columns() == u_expected_matrix.columns());

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::elements_compare(l_matrix, l_expected_matrix, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
		expect(mpp::elements_compare(u_matrix, u_expected_matrix, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
	};
}

template<typename AValue, typename XValue, typename BValue>
void test_substitution(const std::string& filename, const auto& fn)
{
	const auto result_struct =
		parse_data_file_substitution_transformation<AValue, XValue, BValue>(get_filepath(filename));
	const auto& a                 = result_struct.a;
	const auto& b                 = result_struct.b;
	const auto& x_expected_matrix = result_struct.x;

	test(filename) = [&]() {
		const auto x_matrix = fn(a, b);

		expect(x_matrix.rows() == x_expected_matrix.rows());
		expect(x_matrix.columns() == x_expected_matrix.columns());

		using ordering_type = std::compare_three_way_result_t<XValue, XValue>;

		expect(mpp::elements_compare(x_matrix, x_expected_matrix, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
	};
}

int main()
{
	feature("Determinant") = []() {
		test_determinant<int, int>("test_data/0x0_det.txt");
		test_determinant<int, float>("test_data/1x1_det.txt");
		test_determinant<int, double>("test_data/2x2_det.txt");
		test_determinant<int, double>("test_data/10x10_det.txt");
	};

	feature("Transpose") = []() {
		test_transformation<int, int>("test_data/25x25_transpose.txt", mpp::transpose);
		test_transformation<int, int>("test_data/50x2_transpose.txt", mpp::transpose);
	};

	feature("LU Decomposition") = []() {
		test_lu_decomposition<int, double>("test_data/2x2_lu.txt");
		test_lu_decomposition<int, double>("test_data/3x3_lu.txt");
	};

	feature("Inverse") = []() {
		auto inverse_fn = std::bind_front(mpp::inverse, std::type_identity<double>{});

		test_transformation<int, double>("test_data/2x2_inv.txt", inverse_fn);
		test_transformation<int, double>("test_data/3x3_inv.txt", inverse_fn);
		test_transformation<int, double>("test_data/3x3_inv_int.txt", inverse_fn);
		test_transformation<double, double>("test_data/10x10_inv.txt", inverse_fn);
	};

	feature("Block") = []() {
		test_block<int, float>("test_data/4x4_block.txt");
	};

	feature("Forward substitution") = []() {
		auto fwd_substitute_fn = std::bind_front(mpp::forward_substitution, std::type_identity<int>{});

		test_substitution<int, int, int>("test_data/4x4_4x1_fwd_substitution.txt", fwd_substitute_fn);
	};

	feature("Back substitution") = []() {
		auto back_substitution_fn = std::bind_front(mpp::back_substitution, std::type_identity<double>{});

		test_substitution<int, double, int>("test_data/3x3_3x1_back_substitution.txt", back_substitution_fn);
	};

	return 0;
}
