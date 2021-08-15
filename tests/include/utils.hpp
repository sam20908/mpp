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

#include <mpp/detail/util/algo_impl.hpp>
#include <mpp/mat/matfwd.hpp>
#include <mpp/util/cmp.hpp>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

template<typename T, typename... Bufs>
using mats = std::tuple<std::type_identity<mpp::mat<T, Bufs>>...>;

template<typename T, std::size_t Rows, std::size_t Cols>
using fixed_mat = mats<T, std::array<T, Rows * Cols>>;

template<typename T>
using dyn_mat = mats<T, std::vector<T>>;

template<typename T, std::size_t Rows, std::size_t Cols>
using all_mats = mats<T, std::vector<T>, std::array<T, Rows * Cols>>;

template<typename T, std::size_t Rows, std::size_t Cols>
using all_mats_reverse = mats<T, std::array<T, Rows * Cols>, std::vector<T>>;

template<typename...>
struct join_mats_impl;

template<typename... Mats, typename... Mats2>
struct join_mats_impl<std::tuple<Mats...>, std::tuple<Mats2...>>
{
	using type =
		std::tuple<std::tuple<std::type_identity<typename Mats::type>, std::type_identity<typename Mats2::type>>...>;
};

template<typename... Mats, typename... Mats2, typename... Mats3>
struct join_mats_impl<std::tuple<Mats...>, std::tuple<Mats2...>, std::tuple<Mats3...>>
{
	using type = std::tuple<std::tuple<std::type_identity<typename Mats::type>,
		std::type_identity<typename Mats2::type>,
		std::type_identity<typename Mats3::type>>...>;
};

template<typename... Mats>
using join_mats = typename join_mats_impl<Mats...>::type;

auto cmp_mat_to_rng_impl(const auto& mat, const auto& rng) -> bool
{
	const auto expected_rows = rng.size();
	const auto expected_cols = expected_rows == 0 ? 0 : rng[0].size();

	const auto rows_is_eq = mat.rows() == expected_rows;
	const auto cols_is_eq = mat.cols() == expected_cols;

	if (!rows_is_eq || !cols_is_eq)
	{
		return false;
	}

	for (auto row = std::size_t{}; row < expected_rows; ++row)
	{
		for (auto col = std::size_t{}; col < expected_cols; ++col)
		{
			if (mpp::cmp_fn(mat(row, col), rng[row][col]) != 0)
			{
				return false;
			}
		}
	}

	return true;
}

auto cmp_mat_to_expr_like_impl(const auto& mat, const auto& expr) -> bool
{
	const auto rows_is_eq = mat.rows() == expr.rows();
	const auto cols_is_eq = mat.cols() == expr.cols();

	if (!rows_is_eq || !cols_is_eq)
	{
		return false;
	}

	for (auto row = std::size_t{}; row < mat.rows(); ++row)
	{
		for (auto col = std::size_t{}; col < mat.cols(); ++col)
		{
			if (mpp::cmp_fn(mat(row, col), expr(row, col)) != 0)
			{
				return false;
			}
		}
	}

	return true;
}

template<typename Num, typename Num2>
void cmp_nums(Num num, Num2 num2)
{
	using common_val_t = std::common_type_t<Num, Num2>;
	using ut_val_t     = boost::ut::detail::template value<common_val_t>;

	using boost::ut::eq;
	using boost::ut::expect;

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
	using boost::ut::type;
	using boost::ut::operator==;

	boost::ut::expect(type<Mat> == type<Mat2>);
}

void cmp_mat_to_expr_like(const auto& mat, const auto& expr)
{
	boost::ut::expect(cmp_mat_to_expr_like_impl(mat, expr));
}

void cmp_mat_to_rng(const auto& mat, const auto& rng)
{
	boost::ut::expect(cmp_mat_to_rng_impl(mat, rng));
}

template<typename T>
auto str_fn()
{
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
inline auto parse_rng2d = [](std::ifstream& file, std::string& line) {
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

inline auto parse_dims = [](std::ifstream& file, std::string& line) {
	std::getline(file, line);

	auto stream = std::istringstream{ line };
	std::size_t rows, cols;
	stream >> rows >> cols;

	std::getline(file, line); // to skip '='

	return std::pair{ rows, cols };
};

inline auto parse_mat_construct_args = []<typename Mat, typename... Args>(std::type_identity<Mat>, const Args&... args)
{
	return [&](std::ifstream&, std::string&) {
		if constexpr (sizeof...(Args) == 0)
		{
			Mat mat;
			return mat;
		}
		else
		{
			Mat mat{ args... };
			return mat;
		}
	};
};

template<bool Move>
inline auto parse_mat_construct_rng2d = []<typename Mat>(std::type_identity<Mat>) {
	return [&](std::ifstream& file, std::string& line) {
		auto rng = parse_rng2d<typename Mat::value_type>(file, line);

		if constexpr (Move)
		{
			return Mat{ std::move(rng) };
		}
		else
		{
			return Mat{ rng };
		}
	};
};

template<bool Move>
inline auto parse_mat_construct_rng1d = []<typename Mat>(std::type_identity<Mat>) {
	return [&](std::ifstream& file, std::string& line) {
		const auto [rows, cols] = parse_dims(file, line);
		auto rng                = parse_vec1d<typename Mat::value_type>(file, line);

		if constexpr (Move)
		{
			return Mat{ rows, cols, std::move(rng) };
		}
		else
		{
			return Mat{ rows, cols, rng };
		}
	};
};

template<typename T>
inline auto parse_val = [](std::ifstream& file, std::string& line) {
	std::getline(file, line);

	const auto str_val_fn = str_fn<T>();
	const auto val        = str_val_fn(line);

	std::getline(file, line); // to skip '='

	return val;
};

// struct unknown_ordering_t
// {
// };

// template<typename Ordering>
// inline auto parse_ordering = [](std::ifstream& file, std::string& line) {
// 	std::getline(file, line);

// 	auto ordering = [&]() -> std::variant<Ordering, unknown_ordering_t> {
// 		if constexpr (std::is_same_v<Ordering, std::partial_ordering>)
// 		{
// 			if (line == "unordered")
// 			{
// 				return std::partial_ordering::unordered;
// 			}
// 		}

// 		if (line == "greater")
// 		{
// 			return Ordering::greater;
// 		}
// 		else if (line == "less")
// 		{
// 			return Ordering::less;
// 		}
// 		else if (line == "equivalent")
// 		{
// 			return Ordering::equivalent;
// 		}
// 		else
// 		{
// 			return unknown_ordering_t{};
// 		}
// 	}();

// 	std::getline(file, line); // to skip '='

// 	if (ordering.index() == 1)
// 	{
// 		throw std::runtime_error("Unknown ordering for current ordering type");
// 	}

// 	return std::get<0>(ordering);
// };

// struct block_idxs_container
// {
// 	std::size_t row_start;
// 	std::size_t col_start;
// 	std::size_t row_end;
// 	std::size_t col_end;
// };

// inline auto parse_block_idxs = [](std::ifstream& file, std::string& line) {
// 	std::getline(file, line);

// 	auto stream = std::istringstream{ line };
// 	std::size_t row_start, col_start, row_end, col_end;
// 	stream >> row_start >> col_start >> row_end >> col_end;

// 	std::getline(file, line); // to skip '='

// 	return block_idxs_container{ row_start, col_start, row_end, col_end };
// };

template<typename... Fns>
auto parse_test(std::string_view filename, const Fns&... fns)
{
	auto line = std::string{};
	auto file = std::ifstream(get_path_str(filename));

	return std::tuple{ fns(file, line)... };
}
