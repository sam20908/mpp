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

#include <mpp/detail/matrix/matrix_def.hpp>
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
#include <variant>
#include <vector>

namespace mpp
{
	enum class matrix_type;
} // namespace mpp

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using all_mats_t = std::tuple<std::type_identity<mpp::matrix<T, Rows, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, Rows, mpp::dynamic, Alloc...>>>;

template<typename T, std::size_t Rows, std::size_t Columns, typename... Alloc>
using dyn_mats_t = std::tuple<std::type_identity<mpp::matrix<T, mpp::dynamic, mpp::dynamic, Alloc...>>,
	std::type_identity<mpp::matrix<T, mpp::dynamic, Columns, Alloc...>>,
	std::type_identity<mpp::matrix<T, Rows, mpp::dynamic, Alloc...>>>;

template<typename T, std::size_t Rows, std::size_t Columns>
using fixed_mat_t = std::tuple<std::type_identity<mpp::matrix<T, Rows, Columns>>>;

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

template<typename T, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Alloc>
auto stringify_mat(const mpp::matrix<T, RowsExtent, ColumnsExtent, Alloc>& mat)
{
	auto str_stream = std::ostringstream{};

	str_stream << "mpp::matrix<" << str_for_t<T>() << ", " << RowsExtent << ", " << ColumnsExtent << "> [" << mat.rows()
			   << "x" << mat.columns() << "]\n";

	return str_stream;
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
inline auto parse_mat = [](const auto&... args) {
	return [&](std::ifstream& file, std::string& line) {
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

		return Mat{ std::move(data), args... };
	};
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
