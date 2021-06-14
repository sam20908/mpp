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

#include <boost/ut.hpp>

#include <mpp/matrix.hpp>
#include <mpp/utility.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_parsers.hpp"

#include <compare>
#include <concepts>
#include <cstddef>
#include <utility>

using namespace boost::ut::bdd;
using namespace boost::ut;

inline auto stringify_type = [](mpp::matrix_type mat_type) {
	switch (mat_type)
	{
	case mpp::matrix_type::fully_static:
		return "matrix_type::fully_static";
		break;
	case mpp::matrix_type::fully_dynamic:
		return "matrix_type::fully_dynamic";
		break;
	case mpp::matrix_type::dynamic_rows:
		return "matrix_type::dynamic_rows";
		break;
	case mpp::matrix_type::dynamic_columns:
		return "matrix_type::dynamic_columns";
		break;
		[[unlikely]] default : return "unknown";
	}
};

inline auto stringify_ordering = [](auto ordering) {
	using ordering_t = std::remove_cvref_t<decltype(ordering)>;

	if constexpr (std::is_same_v<ordering_t, std::strong_ordering>)
	{
		if (ordering == std::strong_ordering::greater)
		{
			return "strong_ordering::greater";
		}
		else if (ordering == std::strong_ordering::less)
		{
			return "strong_ordering::less";
		}
		else if (ordering == std::strong_ordering::equivalent)
		{
			return "strong_ordering::equivalent";
		}
		else
		{
			return "unknown";
		}
	}
	else if constexpr (std::is_same_v<ordering_t, std::weak_ordering>)
	{
		if (ordering == std::weak_ordering::greater)
		{
			return "weak_ordering::greater";
		}
		else if (ordering == std::weak_ordering::less)
		{
			return "weak_ordering::less";
		}
		else if (ordering == std::weak_ordering::equivalent)
		{
			return "weak_ordering::equivalent";
		}
		else
		{
			return "";
		}
	}
	else if constexpr (std::is_same_v<ordering_t, std::partial_ordering>)
	{
		if (ordering == std::partial_ordering::greater)
		{
			return "partial_ordering::greater";
		}
		else if (ordering == std::partial_ordering::less)
		{
			return "partial_ordering::less";
		}
		else if (ordering == std::partial_ordering::equivalent)
		{
			return "partial_ordering::equivalent";
		}
		else if (ordering == std::partial_ordering::unordered)
		{
			return "partial_ordering::unordered";
		}
		else
		{
			return "unknown";
		}
	}
	else
	{
		return "unknown";
	}
};

inline auto stringify_noop = [](const auto& arg) {
	return arg;
};

template<typename Mat, typename Val, typename Fn, typename StringifyFn>
void test_fn(std::string_view filename, const Fn& fn, const StringifyFn& stringify_fn)
{
	test(filename.data()) = [=]() {
		const auto [mat, expected_val] = parse_test(filename, parse_mat<Mat>(), parse_val<Val>);
		const auto out                 = fn(mat);

		expect(out == expected_val) << "Output is" << stringify_fn(out) << "but expected output is"
									<< stringify_fn(expected_val);
	};
}

template<typename Mat, typename Mat2>
void test_cmp_size(std::string_view filename)
{
	test(filename.data()) = [=]() {
		const auto [mat, mat2, cmp_rows, cmp_cols, expected_row_ordering, expected_col_ordering] = parse_test(filename,
			parse_mat<Mat>(),
			parse_mat<Mat2>(),
			parse_val<bool>,
			parse_val<bool>,
			parse_ordering<std::partial_ordering>,
			parse_ordering<std::partial_ordering>);

		const auto [out_row_ordering, out_col_ordering] = mpp::size_compare(mat, mat2, cmp_rows, cmp_cols);

		expect(out_row_ordering == expected_row_ordering)
			<< "Output is" << stringify_ordering(out_row_ordering) << "but expected output is"
			<< stringify_ordering(expected_row_ordering);
		expect(out_col_ordering == expected_col_ordering)
			<< "Output is" << stringify_ordering(out_col_ordering) << "but expected output is"
			<< stringify_ordering(expected_col_ordering);
	};
}

template<typename Mat, typename Mat2, typename Ordering>
void test_cmp_elems(std::string_view filename)
{
	test(filename.data()) = [=]() {
		const auto [mat, mat2, expected_ordering] =
			parse_test(filename, parse_mat<Mat>(), parse_mat<Mat2>(), parse_ordering<Ordering>);

		const auto out_ordering = mpp::elements_compare(mat, mat2, mpp::floating_point_compare);

		expect(out_ordering == expected_ordering) << "Output is" << stringify_ordering(out_ordering)
												  << "but expected output is" << stringify_ordering(expected_ordering);
	};
}

int main()
{
	using namespace mpp;

	feature("Type") = []() {
		test_fn<matrix<int, 0, 0>, matrix_type>("utility/type/fixed.txt", mpp::type, stringify_type);
		test_fn<matrix<int>, matrix_type>("utility/type/dyn.txt", mpp::type, stringify_type);
		test_fn<matrix<int, dynamic, 0>, matrix_type>("utility/type/dyn_rows.txt", mpp::type, stringify_type);
		test_fn<matrix<int, 0, dynamic>, matrix_type>("utility/type/dyn_cols.txt", mpp::type, stringify_type);
	};

	feature("Square") = []() {
		test_fn<matrix<int, 1, 1>, bool>("utility/sq/1x1.txt", square, stringify_noop);
		test_fn<matrix<int>, bool>("utility/sq/1x2.txt", square, stringify_noop);
		test_fn<matrix<int, dynamic, 3>, bool>("utility/sq/3x3.txt", square, stringify_noop);
		test_fn<matrix<int, 3, dynamic>, bool>("utility/sq/3x2.txt", square, stringify_noop);
	};

	feature("Singular") = []() {
		test_fn<matrix<int, 0, 0>, bool>("utility/sg/0x0.txt", mpp::singular, stringify_noop);
		test_fn<matrix<int>, bool>("utility/sg/1x1.txt", mpp::singular, stringify_noop);
		test_fn<matrix<int, dynamic, 2>, bool>("utility/sg/2x2.txt", mpp::singular, stringify_noop);
		test_fn<matrix<int, 3, dynamic>, bool>("utility/sg/3x3.txt", mpp::singular, stringify_noop);
	};

	feature("Size comparison") = []() {
		test_cmp_size<matrix<int, 0, 0>, matrix<double, 0, dynamic>>("utility/cmp_size/0x0_0x0.txt");
		test_cmp_size<matrix<int>, matrix<int, dynamic, 2>>("utility/cmp_size/1x1_1x2.txt");
		test_cmp_size<matrix<int, dynamic, 4>, matrix<int>>("utility/cmp_size/3x4_3x3.txt");
		test_cmp_size<matrix<int, 4, dynamic>, matrix<int, 4, 4>>("utility/cmp_size/4x4_4x4.txt");
	};

	feature("Elements compare") = []() { // @TODO: Test elements that return other ordering type
		test_cmp_elems<matrix<int, 0, 0>, matrix<double, 0, dynamic>, std::strong_ordering>(
			"utility/cmp_elems/0x0_0x0.txt");
		test_cmp_elems<matrix<int>, matrix<int, dynamic, 1>, std::strong_ordering>("utility/cmp_elems/1x1_1x1.txt");
		test_cmp_elems<matrix<int, dynamic, 3>, matrix<int>, std::strong_ordering>("utility/cmp_elems/3x3_3x3.txt");
		test_cmp_elems<matrix<double, 4, dynamic>, matrix<double, 4, 4>, std::partial_ordering>(
			"utility/cmp_elems/4x4_4x4.txt");
	};

	return 0;
}
