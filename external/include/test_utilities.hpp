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

#pragma once

#include <boost/ut.hpp>

#include <mpp/detail/matrix/matrix_def.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/public.hpp>
#include <mpp/matrix.hpp>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

namespace mpp
{
	enum class matrix_type;

	template<typename T,
		typename T2,
		std::size_t RowsExtent,
		std::size_t RowsExtent2,
		std::size_t ColumnsExtent,
		std::size_t ColumnsExtent2,
		typename Alloc,
		typename Alloc2>
	auto operator==(const matrix<T, RowsExtent, ColumnsExtent, Alloc>& mat,
		const matrix<T2, RowsExtent2, ColumnsExtent2, Alloc2>& mat2) noexcept(noexcept(elements_compare(mat, mat2)))
		-> bool
	{
		return elements_compare(mat, mat2, floating_point_compare) == 0;
	}
} // namespace mpp

using namespace boost::ut;

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using all_mats = std::tuple<std::type_identity<mpp::matrix<T, Rows, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, Rows, mpp::dynamic, Alloc...>>>;

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using dyn_mats = std::tuple<std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, Rows, mpp::dynamic, Alloc...>>>;

template<typename T, std::size_t Rows, std::size_t Columns>
using fixed_mat = std::tuple<std::type_identity<mpp::matrix<T, Rows, Columns>>>;

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using all_trps_mats = std::tuple<std::type_identity<mpp::matrix<T, Columns, Rows, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, Columns, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Rows, Alloc...>>>;

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using dyn_trps_mats = std::tuple<std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, Columns, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Rows, Alloc...>>>;

template<typename T, std::size_t ARows, std::size_t AColumns, std::size_t BColumns, typename... Alloc>
using all_sub_mats = std::tuple<
	std::type_identity<mpp::matrix<T, mpp::detail::prefer_static_extent(ARows, AColumns), BColumns, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::detail::prefer_static_extent(ARows, AColumns), BColumns, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::detail::prefer_static_extent(ARows, AColumns), mpp::dynamic, Alloc...>>>;

template<typename T, std::size_t ARows, std::size_t AColumns, std::size_t BColumns, typename... Alloc>
using dyn_sub_mats = std::tuple<std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::detail::prefer_static_extent(ARows, AColumns), BColumns, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::detail::prefer_static_extent(ARows, AColumns), mpp::dynamic, Alloc...>>>;

template<typename T,
	std::size_t Rows,
	std::size_t Columns,
	typename TopRowIndex,
	typename TopColumnIndex,
	typename BottomRowIndex,
	typename BottomColumnIndex,
	typename... Alloc>
using all_block_mats = std::tuple<std::type_identity<mpp::detail::block_mat_ret_t<T,
									  Rows,
									  Columns,
									  mpp::detail::get_constant_val_or_dynamic<TopRowIndex>(),
									  mpp::detail::get_constant_val_or_dynamic<TopColumnIndex>(),
									  mpp::detail::get_constant_val_or_dynamic<BottomRowIndex>(),
									  mpp::detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
									  Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::detail::block_mat_ret_t<T,
		mpp::dynamic,
		Columns,
		mpp::detail::get_constant_val_or_dynamic<TopRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<TopColumnIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
		Alloc...>>,
	std::type_identity<mpp::detail::block_mat_ret_t<T,
		Rows,
		mpp::dynamic,
		mpp::detail::get_constant_val_or_dynamic<TopRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<TopColumnIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
		Alloc...>>>;

template<typename T,
	std::size_t Rows,
	std::size_t Columns,
	typename TopRowIndex,
	typename TopColumnIndex,
	typename BottomRowIndex,
	typename BottomColumnIndex,
	typename... Alloc>
using dyn_block_mats = std::tuple<std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::detail::block_mat_ret_t<T,
		mpp::dynamic,
		Columns,
		mpp::detail::get_constant_val_or_dynamic<TopRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<TopColumnIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
		Alloc...>>,
	std::type_identity<mpp::detail::block_mat_ret_t<T,
		Rows,
		mpp::dynamic,
		mpp::detail::get_constant_val_or_dynamic<TopRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<TopColumnIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomRowIndex>(),
		mpp::detail::get_constant_val_or_dynamic<BottomColumnIndex>(),
		Alloc...>>>;

template<typename... Mats>
using mats_tup = std::tuple<std::type_identity<Mats>...>;

template<typename...>
struct join_mats;

template<typename... Mats, typename... Mats2>
struct join_mats<std::tuple<Mats...>, std::tuple<Mats2...>>
{
	using type = std::tuple<std::tuple<Mats, Mats2>...>;
};

template<typename... Mats, typename... Mats2, typename... Mats3>
struct join_mats<std::tuple<Mats...>, std::tuple<Mats2...>, std::tuple<Mats3...>>
{
	using type = std::tuple<std::tuple<Mats, Mats2, Mats3>...>;
};

template<typename... Mats>
using join_mats_t = typename join_mats<Mats...>::type;

template<typename T>
auto str_for_t()
{
	if constexpr (std::is_same_v<T, int>)
	{
		return "int";
	}
	if constexpr (std::is_same_v<T, double>)
	{
		return "double";
	}
	if constexpr (std::is_same_v<T, float>)
	{
		return "float";
	}
	if constexpr (std::is_same_v<T, long>)
	{
		return "long";
	}
}

template<typename T>
struct is_std_alloc : std::false_type
{
};

template<typename T>
struct is_std_alloc<std::allocator<T>> : std::true_type
{
};

template<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>
auto stringify_mat(const mpp::matrix<T, RowsExtent, ColumnsExtent, Alloc>& mat, const char* end)
{
	auto str_stream  = std::ostringstream{};
	const auto t_str = str_for_t<T>();

	str_stream << "mpp::matrix<" << t_str << ", " << RowsExtent << ", " << ColumnsExtent << ", ";

	if constexpr (is_std_alloc<Alloc>::value)
	{
		str_stream << "std::allocator<" << t_str << ">";
	}
	else
	{
		str_stream << "custom_allocator<" << t_str << ">";
	}

	str_stream << "> [" << mat.rows() << 'x' << mat.columns() << ']' << end;

	return str_stream;
}

template<typename Num, typename Num2>
void cmp_nums(Num num, Num2 num2)
{
	using common_val_t = std::common_type_t<Num, Num2>;
	using ut_val_t     = boost::ut::detail::template value<common_val_t>;

	if constexpr (std::is_floating_point_v<common_val_t>)
	{
		static constexpr auto eps = std::numeric_limits<common_val_t>::epsilon();

		expect(eq(num, ut_val_t{ num2, eps }));
	}
	else
	{
		expect(eq(num, ut_val_t{ num2 }));
	}
}

template<typename Mat, typename Mat2>
void cmp_mat_types(const Mat&, const Mat2&)
{
	expect(boost::ut::type<Mat> == boost::ut::type<Mat2>);
}

template<typename Mat, typename Rng>
void cmp_mat_to_rng(const Mat& mat, const Rng& rng)
{
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
			cmp_nums(mat(row, column), rng[row][column]);
		}
	}
}

template<typename Mat, typename Mat2>
void cmp_mats(const Mat& mat, const Mat2& mat2)
{
	expect(eq(mat, mat2));
}

// template<typename Mat, typename Expr>
// void cmp_mat_to_expr_like(const Mat& mat, const Expr& expr)
// {
// 	const auto expected_rows    = expr.rows();
// 	const auto expected_columns = expr.columns();

// 	const auto rows_is_eq    = mat.rows() == expected_rows;
// 	const auto columns_is_eq = mat.columns() == expected_columns;

// 	expect(rows_is_eq) << "Matrix's rows doesn't match expected rows";
// 	expect(columns_is_eq) << "Matrix's columns doesn't match expected columns";

// 	if (!rows_is_eq || !columns_is_eq)
// 	{
// 		return; // Avoid accessing out of bounds
// 	}

// 	for (auto row = std::size_t{}; row < expected_rows; ++row)
// 	{
// 		for (auto column = std::size_t{}; column < expected_columns; ++column)
// 		{
// 			cmp_nums(mat(row, column), expr(row, column));
// 		}
// 	}
// }

// @TODO: Use .view() for log_mats* when available for compilers

void log_mat(const auto& mat)
{
	const auto mat_str = stringify_mat(mat, "\n");
	boost::ut::log << mat_str.str().c_str();
}

void log_mats2(const auto& mat, const auto& mat2)
{
	const auto mat_str  = stringify_mat(mat, "");
	const auto mat2_str = stringify_mat(mat2, "\n");
	boost::ut::log << mat_str.str().c_str() << "and" << mat2_str.str().c_str();
}

template<typename T>
auto str_fn()
{
	// @NOTE: Could improve performance by using from_chars (but will need GCC 11 for FP)

	if constexpr (std::is_same_v<T, double>)
	{
		return [](const std::string& str) {
			return std::stod(str);
		};
	}
	else if constexpr (std::is_same_v<T, long double>)
	{
		return [](const std::string& str) {
			return std::stold(str);
		};
	}
	else if constexpr (std::is_same_v<T, float>)
	{
		return [](const std::string& str) {
			return std::stof(str);
		};
	}
	else if constexpr (std::is_same_v<T, long>)
	{
		return [](const std::string& str) {
			return std::stol(str);
		};
	}
	else if constexpr (std::is_same_v<T, long long>)
	{
		return [](const std::string& str) {
			return std::stoll(str);
		};
	}
	else if constexpr (std::is_same_v<T, unsigned long>)
	{
		return [](const std::string& str) {
			return std::stoul(str);
		};
	}
	else if constexpr (std::is_same_v<T, unsigned long long>)
	{
		return [](const std::string& str) {
			return std::stoull(str);
		};
	}
	else if constexpr (std::is_same_v<T, int>)
	{
		return [](const std::string& str) {
			return std::stoi(str);
		};
	}
	else if constexpr (std::is_same_v<T, bool>)
	{
		return [](const std::string& str) {
			return static_cast<bool>(std::stoi(str));
		};
	}

	else if constexpr (std::is_same_v<T, mpp::matrix_type>)
	{
		return [](const std::string& str) {
			return static_cast<mpp::matrix_type>(std::stoi(str));
		};
	}
}

inline auto get_path_str(std::string_view filename)
{
	return std::filesystem::path(TEST_DATA_PATH).append(filename).string();
}

template<typename Mat>
inline auto parse_mat = [](std::ifstream& file, std::string& line) {
	using value_type = typename Mat::value_type;

	auto data             = std::vector<std::vector<value_type>>{};
	const auto str_val_fn = str_fn<value_type>();

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto row     = std::vector<value_type>{};
		auto val_str = std::string{};
		auto stream  = std::istringstream{ line };

		while (std::getline(stream, val_str, ' '))
		{
			row.push_back(str_val_fn(val_str));
		}

		data.push_back(std::move(row));
	}

	return Mat{ std::move(data) };
};

template<typename T>
inline auto parse_vec2d = [](std::ifstream& file, std::string& line) {
	auto data             = std::vector<std::vector<T>>{};
	const auto str_val_fn = str_fn<T>();

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto row     = std::vector<T>{};
		auto val_str = std::string{};
		auto stream  = std::istringstream{ line };

		while (std::getline(stream, val_str, ' '))
		{
			row.push_back(str_val_fn(val_str));
		}

		data.push_back(std::move(row));
	}

	return data;
};

inline auto parse_dims = [](std::ifstream& file, std::string& line) {
	std::getline(file, line);

	auto stream = std::istringstream{ line };
	std::size_t rows, cols;
	stream >> rows >> cols;

	std::getline(file, line); // to skip '='

	return std::pair{ rows, cols };
};

template<typename T>
inline auto parse_vec1d = [](std::ifstream& file, std::string& line) {
	auto data             = std::vector<T>{};
	const auto str_val_fn = str_fn<T>();

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto val_str = std::string{};
		auto stream  = std::istringstream{ line };

		while (std::getline(stream, val_str, ' '))
		{
			data.push_back(str_val_fn(val_str));
		}
	}

	return data;
};

template<typename T, std::size_t Rows, std::size_t Columns>
inline auto parse_arr2d = [](std::ifstream& file, std::string& line) {
	auto data             = std::array<std::array<T, Columns>, Rows>{};
	auto row_idx          = std::size_t{};
	const auto str_val_fn = str_fn<T>();

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto row     = std::vector<T>{};
		auto val_str = std::string{};
		auto stream  = std::istringstream{ line };
		auto col_idx = std::size_t{};

		while (std::getline(stream, val_str, ' '))
		{
			data[row_idx][col_idx++] = str_val_fn(val_str);
		}

		++row_idx;
	}

	return data;
};

template<typename T>
inline auto parse_val = [](std::ifstream& file, std::string& line) {
	std::getline(file, line);

	const auto str_val_fn = str_fn<T>();
	const auto val        = str_val_fn(line);

	std::getline(file, line); // to skip '='

	return val;
};

struct unknown_ordering_t
{
};

template<typename Ordering>
inline auto parse_ordering = [](std::ifstream& file, std::string& line) {
	std::getline(file, line);

	auto ordering = [&]() -> std::variant<Ordering, unknown_ordering_t> {
		if constexpr (std::is_same_v<Ordering, std::partial_ordering>)
		{
			if (line == "unordered")
			{
				return std::partial_ordering::unordered;
			}
		}

		if (line == "greater")
		{
			return Ordering::greater;
		}
		else if (line == "less")
		{
			return Ordering::less;
		}
		else if (line == "equivalent")
		{
			return Ordering::equivalent;
		}
		else
		{
			return unknown_ordering_t{};
		}
	}();

	std::getline(file, line); // to skip '='

	if (ordering.index() == 1)
	{
		throw std::runtime_error("Unknown ordering for current ordering type");
	}

	return std::get<0>(ordering);
};

template<typename... Fns>
auto parse_test(std::string_view filename, const Fns&... fns)
{
	auto line = std::string{};
	auto file = std::ifstream(get_path_str(filename));

	return std::tuple{ fns(file, line)... };
}
