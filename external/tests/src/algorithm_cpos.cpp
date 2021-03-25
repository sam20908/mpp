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
#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>

#include "../../include/data_parser.hpp"
#include "../../include/utility.hpp"

// clang-format off
/**
 * @TODO: Remove this whole clang-format off section once
 * https://github.com/boost-ext/ut/issues/432 is resolved
 * 
 * ut.hpp actually has a bug in its headers when __cpp_modules is defined due
 * to how it's closing its namespaces
 */

#include <filesystem>
#include "../../thirdparty/ut.hpp"
// clang-format on

#include <compare>
#include <functional>
#include <string>
#include <utility>

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

// @TODO: DRY the two matrices element compare wise (dependent on #130?)

static auto get_actual_filepath(const std::string& filename) -> std::filesystem::path
{
	return std::filesystem::path(TEST_DATA_PATH) / filename;
}

template<typename Tag = decltype(tag(""))>
auto skip_test_if_file_not_exists(bool file_exists, Tag& test_tag)
{
	if (!file_exists)
	{
		boost::ut::log << "Test skipped because test data file does not exist\n";
		test_tag = tag("skip");
	}
}

template<typename From, typename To, typename Tag = decltype(tag(""))>
void test_determinant(const std::string& filename, Tag test_tag = tag("execute"))
{
	const auto result_struct  = parse_data_file_with_single_number_result<From, To>(get_actual_filepath(filename));
	const auto& original_data = result_struct.original_data;
	const auto result         = result_struct.result;
	const auto file_exists    = result_struct.file_exists;

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		const auto det = mpp::determinant(std::type_identity<To>{}, original_data);

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::floating_point_compare(det, result) == ordering_type::equivalent);
	};
}

template<typename From, typename To, typename Tag = decltype(tag(""))>
void test_transformation(const std::string& filename, const auto& transform_fn, Tag test_tag = tag("execute"))
{
	const auto result_struct       = parse_data_file_matrix_transformation<From, To>(get_actual_filepath(filename));
	const auto& original_matrix    = result_struct.original_matrix;
	const auto& transformed_matrix = result_struct.transformed_matrix;
	const auto file_exists         = result_struct.file_exists;

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		const auto result_matrix = transform_fn(original_matrix);

		expect(result_matrix.rows() == transformed_matrix.rows());
		expect(result_matrix.columns() == transformed_matrix.columns());

		using ordering_type = std::compare_three_way_result_t<To, To>;

		expect(mpp::elements_compare(result_matrix, transformed_matrix, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
	};
}

template<typename From, typename To, typename Tag = decltype(tag(""))>
void test_block(const std::string& filename, Tag test_tag = tag("execute"))
{
	const auto result_struct    = parse_data_file_block_transformation<From, To>(get_actual_filepath(filename));
	const auto& original_matrix = result_struct.original_matrix;
	const auto& blocks          = result_struct.blocks;
	const auto file_exists      = result_struct.file_exists;

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
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

int main()
{
	cfg<override> = { .tag = { "execute" } };

	feature("algorithm CPOs") = []() {
		given("determinant CPO") = []() {
			test_determinant<int, int>("test_data/0x0_det.txt");
			test_determinant<int, float>("test_data/1x1_det.txt");
			test_determinant<double, double>("test_data/2x2_det.txt");

			test_determinant<double, double>("test_data/10x10_det.txt", tag("skip"));
		};

		given("transpose CPO") = []() {
			test_transformation<int, int>("test_data/25x25_transpose.txt", mpp::transpose);
			test_transformation<int, int>("test_data/50x2_transpose.txt", mpp::transpose);
		};

		given("inverse CPO") = []() {
			using to_type = double;

			auto inverse_fn = std::bind_front(mpp::inverse, std::type_identity<to_type>{});

			test_transformation<int, to_type>("test_data/2x2_inv.txt", inverse_fn);

			test_transformation<double, to_type>("test_data/10x10_inv.txt", inverse_fn, tag("skip"));
		};

		given("block CPO") = []() {
			test_block<float, float>("test_data/4x4_block.txt");
		};
	};

	return 0;
}
