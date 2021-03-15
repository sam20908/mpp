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

using value_type = double;

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

template<typename Tag = decltype(tag(""))>
void test_det(const std::string& filename, Tag test_tag = tag("execute"))
{
	const auto result_tuple = parse_data_file_double_det(get_actual_filepath(filename));
	const auto& data        = std::get<0>(result_tuple);
	const auto& result      = std::get<1>(result_tuple);
	const auto& file_exists = std::get<2>(result_tuple);

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		const auto matrix = mpp::matrix<value_type>{ data };
		const auto det    = mpp::determinant(std::type_identity<value_type>{}, matrix);

		using ordering_type = std::compare_three_way_result_t<value_type, value_type>;

		expect(
			mpp::floating_point_compare(det, static_cast<value_type>(result)) == ordering_type::equivalent);
	};
}

template<std::size_t RowsExtent,
	std::size_t ColumnsExtent,
	std::size_t ResultRowsExtent,
	std::size_t ResultColumnsExtent>
void test_transformation_helper(const auto& transform_fn, const auto& result, const auto& data)
{
	const auto result_matrix = mpp::matrix<value_type, ResultRowsExtent, ResultColumnsExtent>{ result };
	const auto matrix        = mpp::matrix<value_type, RowsExtent, ColumnsExtent>{ data };
	const auto transformed   = std::invoke(transform_fn, matrix);

	expect(result_matrix.rows() == transformed.rows());
	expect(result_matrix.columns() == transformed.columns());
	expect(result_matrix.rows_extent() == transformed.rows_extent());
	expect(result_matrix.columns_extent() == transformed.columns_extent());

	using ordering_type = std::compare_three_way_result_t<value_type, value_type>;

	expect(mpp::elements_compare(result_matrix, transformed, mpp::floating_point_compare) ==
		   ordering_type::equivalent);
}

template<std::size_t RowsExtent,
	std::size_t ColumnsExtent,
	std::size_t ResultRowsExtent,
	std::size_t ResultColumnsExtent,
	typename Tag = decltype(tag(""))>
void test_transformation(const std::string& filename, const auto& transform_fn, Tag test_tag = tag("execute"))
{
	// @FIXME: Find a way to avoid reparse the same file when calling this with different extents but with the same
	// filename
	const auto result_tuple = parse_data_file_matrix_transformation_double(get_actual_filepath(filename));
	const auto& data        = std::get<0>(result_tuple);
	const auto& results     = std::get<1>(result_tuple);
	const auto& file_exists = std::get<2>(result_tuple);

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		test_transformation_helper<RowsExtent, ColumnsExtent, ResultRowsExtent, ResultColumnsExtent>(transform_fn,
			results,
			data);
	};
}

template<std::size_t RowsExtent,
	std::size_t ColumnsExtent,
	std::size_t ResultRowsExtent,
	std::size_t ResultColumnsExtent>
void test_block_helper(const auto& results, const auto& data, auto... dimension_args)
{
	const auto matrix = mpp::matrix<value_type, RowsExtent, ColumnsExtent>{ dimension_args..., data };

	for (const auto& result : results)
	{
		const auto row_start_idx = std::get<0>(result);
		const auto col_start_idx = std::get<1>(result);
		const auto row_end_idx   = std::get<2>(result);
		const auto col_end_idx   = std::get<3>(result);
		const auto cropped       = mpp::block(matrix, row_start_idx, col_start_idx, row_end_idx, col_end_idx);
		const auto result_matrix = mpp::matrix<value_type>{ std::get<4>(result) };

		expect(cropped.rows() == result_matrix.rows());
		expect(cropped.columns() == result_matrix.columns());

		using ordering_type = std::compare_three_way_result_t<value_type, value_type>;

		expect(mpp::elements_compare(result_matrix, cropped, mpp::floating_point_compare) ==
			   ordering_type::equivalent);
	}
}

template<typename Tag = decltype(tag(""))>
void test_block(const std::string& filename, Tag test_tag = tag("execute"))
{
	// @FIXME: Find a way to avoid reparse the same file when calling this with different extents but with the same
	// filename
	const auto result_tuple = parse_data_file_matrix_block_double(get_actual_filepath(filename));
	const auto& data        = std::get<0>(result_tuple);
	const auto& results     = std::get<1>(result_tuple);
	const auto& file_exists = std::get<2>(result_tuple);

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		test_block_helper<std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent>(results,
			data);
	};
}

int main()
{
	cfg<override> = { .tag = { "execute" } };

	// @TODO: Several tests are skipped due to #163

	feature("algorithm CPOs") = []() {
		given("determinant CPO") = []() {
			test_det("test_data/0x0_det.txt");
			test_det("test_data/1x1_det.txt");
			test_det("test_data/2x2_det.txt");

			test_det("test_data/10x10_det.txt", tag("skip"));
		};

		given("transpose CPO") = []() {
			test_transformation<25, 25, 25, 25>("test_data/25x25_transpose.txt", mpp::transpose);
			test_transformation<50, 2, 2, 50>("test_data/50x2_transpose.txt", mpp::transpose);
		};

		given("inverse CPO") = []() {
			auto inverse_fn = std::bind_front(mpp::inverse, std::type_identity<value_type>{});

			test_transformation<2, 2, 2, 2>("test_data/2x2_inv.txt", inverse_fn);

			test_transformation<10, 10, 10, 10>("test_data/10x10_inv.txt", inverse_fn, tag("skip"));
		};

		given("block CPO") = []() {
			test_block("test_data/4x4_block.txt");
		};
	};

	return 0;
}
