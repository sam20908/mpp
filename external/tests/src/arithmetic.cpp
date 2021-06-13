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
#include <mpp/arithmetic.hpp>
#include <mpp/matrix.hpp>

#include "../../include/test_parsers.hpp"
#include "../../include/utility.hpp"

using namespace boost::ut;
using namespace boost::ut::bdd;

template<typename Val,
	std::size_t Rows,
	std::size_t Columns,
	typename Val2,
	std::size_t Rows2,
	std::size_t Columns2,
	typename Val3,
	std::size_t Rows3,
	std::size_t Columns3>
void test_mat_op(const std::string& filename, const auto& op)
{
	const auto result =
		parse_test<temp_types<vec2d_t, vec2d_t, vec2d_t>, types<Val, Val2, Val3>>(get_filepath(filename),
			std::tuple{ vec2d_fn, vec2d_fn, vec2d_fn });

	const auto mat_tup      = create_mats<Val, Rows, Columns, all_mats_t>(args(fwd_args, std::get<0>(result)));
	const auto mat_tup2     = create_mats<Val2, Rows2, Columns2, all_mats_t>(args(fwd_args, std::get<1>(result)));
	const auto expected_tup = create_mats<Val3, Rows3, Columns3, all_mats_t>(args(fwd_args, std::get<2>(result)));

	test(filename) = [&]() {
		[&]<std::size_t... Idx, std::size_t... Idx2, std::size_t... Idx3>(std::index_sequence<Idx...>,
			std::index_sequence<Idx2...>,
			std::index_sequence<Idx3...>)
		{
			when("Don't fully evaluate the result") = [&]() {
				(cmp_mat_to_expr_like(std::get<Idx3>(expected_tup),
					 op(std::get<Idx>(mat_tup), std::get<Idx2>(mat_tup2))),
					...);
			};

			when("Fully evaluate the result") = [&]() {
				(cmp_mat_to_expr_like(std::get<Idx3>(expected_tup),
					 mpp::matrix{ op(std::get<Idx>(mat_tup), std::get<Idx2>(mat_tup2)) }),
					...);
			};
		}
		(std::make_index_sequence<4>{}, std::make_index_sequence<4>{}, std::make_index_sequence<4>{});
	};
}

template<typename Val,
	std::size_t Rows,
	std::size_t Columns,
	typename Val2,
	typename Val3,
	std::size_t Rows3,
	std::size_t Columns3>
void test_num_op(const std::string& filename, const auto& op)
{
	const auto result = parse_test<temp_types<vec2d_t, val_t, vec2d_t>, types<Val, Val2, Val3>>(get_filepath(filename),
		std::tuple{ vec2d_fn, val_fn, vec2d_fn });

	const auto num          = std::get<1>(result);
	const auto mat_tup      = create_mats<Val, Rows, Columns, all_mats_t>(args(fwd_args, std::get<0>(result)));
	const auto expected_tup = create_mats<Val3, Rows3, Columns3, all_mats_t>(args(fwd_args, std::get<2>(result)));

	test(filename) = [&]() {
		[&]<std::size_t... Idx, std::size_t... Idx2>(std::index_sequence<Idx...>, std::index_sequence<Idx2...>)
		{
			when("Don't fully evaluate the result") = [&]() {
				(cmp_mat_to_expr_like(std::get<Idx2>(expected_tup), op(std::get<Idx>(mat_tup), num)), ...);
			};

			when("Fully evaluate the result") = [&]() {
				(cmp_mat_to_expr_like(std::get<Idx2>(expected_tup), mpp::matrix{ op(std::get<Idx>(mat_tup), num) }),
					...);
			};
		}
		(std::make_index_sequence<4>{}, std::make_index_sequence<4>{});
	};
}

int main()
{
	test_mat_op<int, 2, 3, int, 2, 3, int, 2, 3>("arithmetic/2x3_add.txt", [](const auto& a, const auto& b) {
		return a + b;
	});

	test_mat_op<int, 2, 3, int, 2, 3, int, 2, 3>("arithmetic/2x3_subtract.txt", [](const auto& a, const auto& b) {
		return a - b;
	});

	test_mat_op<int, 2, 3, int, 3, 1, int, 2, 1>("arithmetic/2x3_3x1_multiply.txt", [](const auto& a, const auto& b) {
		return a * b;
	});

	test_num_op<int, 2, 3, int, int, 2, 3>("arithmetic/2x3_multiply.txt", [](const auto& a, const auto& b) {
		return a * b;
	});

	test_num_op<float, 2, 3, float, float, 2, 3>("arithmetic/2x3_divide.txt", [](const auto& a, const auto& b) {
		return a / b;
	});

	return 0;
}
