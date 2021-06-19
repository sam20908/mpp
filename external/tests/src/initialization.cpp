/***
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

#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include "../../include/custom_allocator.hpp"
#include "../../include/test_utilities.hpp"

#include <array>
#include <compare>
#include <cstddef>
#include <initializer_list>
#include <string_view>
#include <tuple>
#include <vector>

using namespace boost::ut;
using namespace boost::ut::bdd;
using namespace mpp;

template<typename... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};

template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<typename Mat, typename Rng>
void cmp_mat_to_rng(const Mat& mat, const Rng& rng)
{
	using mat_val_t = typename Mat::value_type;
	using rng_val_t = typename Rng::value_type::value_type;
	using ordering  = std::compare_three_way_result_t<mat_val_t, rng_val_t>;

	const auto expected_rows    = rng.size();
	const auto expected_columns = expected_rows == 0 ? 0 : rng[0].size();

	const auto rows_is_eq    = mat.rows() == expected_rows;
	const auto columns_is_eq = mat.columns() == expected_columns;

	expect(rows_is_eq) << "Matrix's rows doesn't match expected rows";
	expect(columns_is_eq) << "Matrix's columns doesn't match expected columns";

	if (!rows_is_eq || !columns_is_eq)
	{
		return; // Avoid accessing out of bounds
	}

	for (auto row = std::size_t{}; row < expected_rows; ++row)
	{
		for (auto column = std::size_t{}; column < expected_columns; ++column)
		{
			expect(mpp::floating_point_compare(mat(row, column), rng[row][column]) == ordering::equivalent)
				<< "Output is" << mat(row, column) << "but expected output is" << rng[row][column] << "at index" << row
				<< column;
		}
	}
}

template<typename Mats, bool CmpAlloc, typename InputFn, typename Alloc, typename... Args>
void test_init(std::string_view test_name,
	const InputFn& input_fn,
	[[maybe_unused]] const Alloc& alloc_obj,
	const Args&... args)
{
	test(test_name.data()) = [&, test_name](const auto& mat_identity) {
		const auto [out, expected_rng] = input_fn(test_name, mat_identity, args...);

		const auto stringified_mat = stringify_mat(out);
		boost::ut::log << stringified_mat.str().c_str(); // @TODO: Use .view() for GCC when available for compilers

		cmp_mat_to_rng(out, expected_rng);

		if constexpr (CmpAlloc)
		{
			expect(out.get_allocator() == alloc_obj) << "Matrix's allocator object doesn't match";
		}
	} | Mats{};
}

// @TODO: Somehow unify test_init_ctor_copy and test_init_ctor_move?

template<typename Mats, typename Mats2, bool CmpAlloc>
void test_init_ctor_copy(std::string_view test_name, [[maybe_unused]] const auto& alloc_obj, const auto&... args)
{
	test(test_name.data()) =
		[&, test_name]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
			std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>) {
			using mat_t = matrix<T, RowsExtent, ColumnsExtent, Alloc>;

			const auto [input_mat, expected_rng] = parse_test(test_name, parse_mat<mat_t>(), parse_vec2d<T>);
			const auto input_mat_name            = stringify_mat(input_mat);

			test("Cross-testing (copy initialization)") =
				[&]<typename T2, std::size_t RowsExtent2, std::size_t ColumnsExtent2, typename Alloc2>(
					std::type_identity<matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>>) {
					using mat2_t = matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>;

					const auto res_mat      = mat2_t{ input_mat, args... };
					const auto res_mat_name = stringify_mat(res_mat);

					boost::ut::log
						<< input_mat_name.str().c_str() << "initialized with"
						<< res_mat_name.str().c_str(); // @TODO: Use .view() for GCC when available for compilers

					cmp_mat_to_rng(res_mat, expected_rng);

					if constexpr (CmpAlloc)
					{
						expect(res_mat.get_allocator() == alloc_obj) << "Matrix's allocator object doesn't match";
					}
				} |
				Mats2{};
		} |
		Mats{};
}

template<typename Mats, typename Mats2, bool CmpAlloc>
void test_init_ctor_move(std::string_view test_name, [[maybe_unused]] const auto& alloc_obj, const auto&... args)
{
	test(test_name.data()) =
		[&, test_name]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
			std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>) {
			using mat_t = matrix<T, RowsExtent, ColumnsExtent, Alloc>;

			const auto [input_vec2d, expected_rng] = parse_test(test_name, parse_vec2d<T>, parse_vec2d<T>);
			const auto input_mat                   = mat_t{ input_vec2d };
			const auto input_mat_name              = stringify_mat(input_mat);

			test("Cross-testing (move initialization)") =
				[&]<typename T2, std::size_t RowsExtent2, std::size_t ColumnsExtent2, typename Alloc2>(
					std::type_identity<matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>>) {
					using mat2_t = matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>;

					auto input_mat2         = mat_t{ input_vec2d }; // Create local copy to move from
					const auto res_mat      = mat2_t{ std::move(input_mat2), args... };
					const auto res_mat_name = stringify_mat(res_mat);

					boost::ut::log
						<< input_mat_name.str().c_str() << "initialized with"
						<< res_mat_name.str().c_str(); // @TODO: Use .view() for GCC when available for compilers

					cmp_mat_to_rng(res_mat, expected_rng);

					if constexpr (CmpAlloc)
					{
						expect(res_mat.get_allocator() == alloc_obj) << "Matrix's allocator object doesn't match";
					}
				} |
				Mats2{};
		} |
		Mats{};
}

int main()
{
	// @NOTE: Construction from expression object will be covered in lazy/eager arithmetic tests
	// @NOTE: This covers copy and move construction from rule of five (2/5)

	using alloc_t        = custom_allocator<double>;
	const auto alloc_obj = alloc_t{};

	feature("Default initialization") = [&]() {
		const auto parse =
			[]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc, typename... Args>(
				std::string_view filename,
				std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
				const Args&... args)
		{
			using mat_t = matrix<T, RowsExtent, ColumnsExtent, Alloc>;

			const auto [expected_rng] = parse_test(filename, parse_vec2d<T>);

			const auto out = [&]() {
				if constexpr (sizeof...(Args) == 0)
				{
					mat_t out; // True default initialization (no parenthesis)
					return out;
				}
				else
				{
					return mat_t{ args... };
				}
			}();

			return std::pair{ out, expected_rng };
		};

		test_init<all_mats_t<int, 0, 0>, false>("initialization/0x0_default.txt", parse, alloc_obj);
		test_init<fixed_mat_t<int, 2, 3>, false>("initialization/2x3_default_fixed.txt", parse, alloc_obj);
		test_init<dyn_mats_t<double, 0, 0, alloc_t>, true>("initialization/0x0_default.txt",
			parse,
			alloc_obj,
			alloc_obj);
	};

	feature("2D range initialization") = [&]() {
		const auto parse =
			[]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(std::string_view filename,
				std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
				const auto&... args) {
				const auto [init_rng, expected_rng] = parse_test(filename, parse_arr2d<T, 2, 3>, parse_vec2d<T>);

				const auto out = matrix<T, RowsExtent, ColumnsExtent, Alloc>{ init_rng, args... };

				return std::pair{ out, expected_rng };
			};

		test_init<all_mats_t<int, 2, 3>, false>("initialization/2x3_rng_2d.txt", parse, alloc_obj);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_rng_2d.txt", parse, alloc_obj);
		test_init<all_mats_t<int, 2, 3>, false>("initialization/2x3_rng_2d.txt", parse, alloc_obj, unsafe);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_rng_2d.txt",
			parse,
			alloc_obj,
			unsafe,
			alloc_obj);
	};

	feature("1D range initialization") = [&]() {
		const auto parse = []<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
							   std::string_view filename,
							   std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
							   const auto&... args) {
			const auto [dims, init_rng, expected_rng] =
				parse_test(filename, parse_dims, parse_vec1d<T>, parse_vec2d<T>);

			const auto out = matrix<T, RowsExtent, ColumnsExtent, Alloc>{ dims.first, dims.second, init_rng, args... };

			return std::pair{ out, expected_rng };
		};

		test_init<all_mats_t<int, 2, 3>, false>("initialization/2x3_rng_1d.txt", parse, alloc_obj);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_rng_1d.txt", parse, alloc_obj);
		test_init<all_mats_t<int, 2, 3>, false>("initialization/2x3_rng_1d.txt", parse, alloc_obj, unsafe);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_rng_1d.txt",
			parse,
			alloc_obj,
			unsafe,
			alloc_obj);
	};

	const auto init_mat_extent_dependent = overloaded{
		// clang-format off
		[]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(
			std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>>,
			[[maybe_unused]] std::size_t rows,
			[[maybe_unused]] std::size_t columns,
			const auto&... args) {
			return matrix<T, RowsExtent, ColumnsExtent, Alloc>{ args... };
		},
		[]<typename T, typename Alloc>(std::type_identity<matrix<T, dynamic, dynamic, Alloc>>,
			[[maybe_unused]] std::size_t rows,
			[[maybe_unused]] std::size_t columns,
			const auto&... args) {
			return matrix<T, dynamic, dynamic, Alloc>{ rows, columns, args... };
		},
		[]<typename T, std::size_t ColumnsExtent, typename Alloc>(
			std::type_identity<matrix<T, dynamic, ColumnsExtent, Alloc>>,
			[[maybe_unused]] std::size_t rows,
			[[maybe_unused]] std::size_t columns,
			const auto&... args) {
			return matrix<T, dynamic, ColumnsExtent, Alloc>{ rows, args... };
		},
		[]<typename T, std::size_t RowsExtent, typename Alloc>(
			std::type_identity<matrix<T, RowsExtent, dynamic, Alloc>>,
			[[maybe_unused]] std::size_t rows,
			[[maybe_unused]] std::size_t columns,
			const auto&... args) {
			return matrix<T, RowsExtent, dynamic, Alloc>{ columns, args... };
		}
		// clang-format on
	};

	const auto parse_extent_dependent =
		[&]<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>(std::string_view filename,
			std::type_identity<matrix<T, RowsExtent, ColumnsExtent, Alloc>> identity,
			const auto&... args) {
			const auto [dims, expected_rng] = parse_test(filename, parse_dims, parse_vec2d<T>);

			const auto out = init_mat_extent_dependent(identity, dims.first, dims.second, args...);

			return std::pair{ out, expected_rng };
		};

	feature("Initialization via callable return values") = [&]() {
		const auto fn = []() {
			return 1.0;
		};

		test_init<all_mats_t<double, 2, 3>, false>("initialization/2x3_callable.txt",
			parse_extent_dependent,
			alloc_obj,
			fn);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_callable.txt",
			parse_extent_dependent,
			alloc_obj,
			fn);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_callable.txt",
			parse_extent_dependent,
			alloc_obj,
			fn,
			alloc_obj);
	};

	feature("Initialization via value") = [&]() {
		const auto val = 2.0;

		test_init<all_mats_t<double, 2, 3>, false>("initialization/2x3_val.txt",
			parse_extent_dependent,
			alloc_obj,
			val);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_val.txt",
			parse_extent_dependent,
			alloc_obj,
			val);
		test_init<dyn_mats_t<double, 2, 3, alloc_t>, true>("initialization/2x3_val.txt",
			parse_extent_dependent,
			alloc_obj,
			val,
			alloc_obj);
	};

	feature("Initialization of identity matrices") = [&]() {
		test_init<all_mats_t<double, 3, 3>, false>("initialization/3x3_identity.txt",
			parse_extent_dependent,
			alloc_obj,
			identity);
		test_init<dyn_mats_t<double, 3, 3, alloc_t>, true>("initialization/3x3_identity.txt",
			parse_extent_dependent,
			alloc_obj,
			identity);
		test_init<all_mats_t<double, 3, 3>, false>("initialization/3x3_identity.txt",
			parse_extent_dependent,
			alloc_obj,
			identity,
			unsafe);
		test_init<dyn_mats_t<double, 3, 3, alloc_t>, true>("initialization/3x3_identity.txt",
			parse_extent_dependent,
			alloc_obj,
			identity,
			unsafe,
			0.0,
			1.0,
			alloc_obj);
	};

	feature("Copy initialization") = [&]() {
		test_init_ctor_copy<all_mats_t<int, 2, 3>, all_mats_t<int, 2, 3>, false>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj);
		test_init_ctor_copy<dyn_mats_t<double, 2, 3, alloc_t>, dyn_mats_t<double, 2, 3, alloc_t>, true>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			alloc_obj);
		test_init_ctor_copy<all_mats_t<int, 2, 3>, all_mats_t<int, 2, 3>, false>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			unsafe);
		test_init_ctor_copy<dyn_mats_t<double, 2, 3, alloc_t>, dyn_mats_t<double, 2, 3, alloc_t>, true>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			unsafe,
			alloc_obj);
	};

	feature("Move initialization") = [&]() {
		test_init_ctor_move<all_mats_t<int, 2, 3>, all_mats_t<int, 2, 3>, false>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj);
		test_init_ctor_move<dyn_mats_t<double, 2, 3, alloc_t>, dyn_mats_t<double, 2, 3, alloc_t>, true>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			alloc_obj);
		test_init_ctor_move<all_mats_t<int, 2, 3>, all_mats_t<int, 2, 3>, false>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			unsafe);
		test_init_ctor_move<dyn_mats_t<double, 2, 3, alloc_t>, dyn_mats_t<double, 2, 3, alloc_t>, true>(
			"initialization/2x3_init_copy_and_move.txt",
			alloc_obj,
			unsafe,
			alloc_obj);
	};

	return 0;
}
