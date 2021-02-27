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

#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>

#include "../../thirdparty/ut.hpp"
#include "../include/data_parser.hpp"
#include "../include/utility.hpp"

#include <filesystem>
#include <functional>
#include <string>
#include <utility>

using namespace boost::ut::literals;
using namespace boost::ut::operators::terse;
using namespace boost::ut::bdd;
using namespace boost::ut;

using value_type = double;

auto get_actual_filepath(const std::string& filename) -> std::filesystem::path
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
	const auto [data, result, file_exists] = parse_data_file_double_det(get_actual_filepath(filename));

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		const auto matrix = mpp::matrix<value_type>{ data };
		const auto det    = mpp::determinant(std::type_identity<value_type>{}, matrix);

		expect(accurate_equals(det, static_cast<value_type>(result)));
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

	const auto all_elems_equal =
		std::ranges::equal(result_matrix, transformed, [](const auto& left, const auto& right) {
			return accurate_equals(left, right);
		});

	expect(all_elems_equal);
}

template<std::size_t RowsExtent,
	std::size_t ColumnsExtent,
	typename ResultExtentsGetter = decltype([](std::size_t left, std::size_t right) {
		return std::pair{ left, right };
	}),
	typename Tag                 = decltype(tag(""))>
void test_transformation(const std::string& filename, const auto& transform_fn, Tag test_tag = tag("execute"))
{
	const auto [data, result, file_exists] =
		parse_data_file_matrix_transformation_double(get_actual_filepath(filename));

	skip_test_if_file_not_exists(file_exists, test_tag);

	test_tag / test(filename) = [&]() {
		constexpr auto result_rows_extent = ResultExtentsGetter{}(RowsExtent, ColumnsExtent).first;
		constexpr auto result_cols_extent = ResultExtentsGetter{}(RowsExtent, ColumnsExtent).second;

		test_transformation_helper<RowsExtent, ColumnsExtent, result_rows_extent, result_cols_extent>(transform_fn,
			result,
			data);

		test_transformation_helper<std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent>(
			transform_fn,
			result,
			data);

		constexpr auto result_rows_extent_2 = ResultExtentsGetter{}(std::dynamic_extent, ColumnsExtent).first;
		constexpr auto result_cols_extent_2 = ResultExtentsGetter{}(std::dynamic_extent, ColumnsExtent).second;

		test_transformation_helper<std::dynamic_extent, ColumnsExtent, result_rows_extent_2, result_cols_extent_2>(
			transform_fn,
			result,
			data);

		constexpr auto result_rows_extent_3 = ResultExtentsGetter{}(RowsExtent, std::dynamic_extent).first;
		constexpr auto result_cols_extent_3 = ResultExtentsGetter{}(RowsExtent, std::dynamic_extent).second;

		test_transformation_helper<RowsExtent, std::dynamic_extent, result_rows_extent_3, result_cols_extent_3>(
			transform_fn,
			result,
			data);
	};
}

int main()
{
	cfg<override> = { .tag = { "execute" } };

	// @TODO: Several tests are skipped due to #163
	// @TODO: Add test cases for block

	feature("algorithm CPOs") = []() {
		given("determinant CPO") = []() {
			test_det("test_data/0x0_det.txt");
			test_det("test_data/1x1_det.txt");
			test_det("test_data/2x2_det.txt");

			test_det("test_data/10x10_det.txt", tag("skip"));
		};

		given("transpose CPO") = []() {
			using flip_extent_fn = decltype([](std::size_t left, std::size_t right) {
				return std::pair{ right, left };
			});

			test_transformation<25, 25, flip_extent_fn>("test_data/25x25_transpose.txt", mpp::transpose);
			test_transformation<50, 2, flip_extent_fn>("test_data/50x2_transpose.txt", mpp::transpose);
		};

		given("inverse CPO") = []() {
			auto inverse_fn = std::bind_front(mpp::inverse, std::type_identity<value_type>{});

			test_transformation<2, 2>("test_data/2x2_inv.txt", inverse_fn);

			test_transformation<10, 10>("test_data/10x10_inv.txt", inverse_fn, tag("skip"));
		};
	};

	return 0;
}