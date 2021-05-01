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

#include <mpp/matrix.hpp>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

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
}

inline auto get_filepath(const std::string& filename) -> std::filesystem::path
{
	return std::filesystem::path(TEST_DATA_PATH) / filename;
}

inline auto mat_fn = []<typename T>(std::size_t, std::size_t, auto&& data) {
	return mpp::matrix<T>{ std::forward<decltype(data)>(data) };
};

inline auto vec2d_fn = []<typename T>(std::size_t, std::size_t, auto&& data) {
	return std::move(data);
};

inline auto vec1d_fn = []<typename T>(std::size_t, std::size_t, auto&& data) {
	return data[0];
};

inline auto val_fn = []<typename T>(std::size_t, std::size_t, auto&& data) {
	return data[0][0];
};

template<typename T>
using mat_t = mpp::matrix<T>;

template<typename T>
using vec2d_t = std::vector<std::vector<T>>;

template<typename T>
using vec1d_t = std::vector<T>;

template<typename T>
using val_t = T;

template<typename... Ts>
struct types
{
	static constexpr auto size = sizeof...(Ts);
};

template<template<typename> typename... Ts>
struct temp_types
{
	static constexpr auto size = sizeof...(Ts);
};

template<typename T>
auto parse_mat_elems(std::ifstream& file, std::string& line, std::size_t rows, std::size_t columns, const auto& fn)
{
	/**
	 * put the result into a 2D vector because it allows you to make a matrix out of it and extract a 1D vector out of
	 * it (just vec[0]). This was meant to be as generic as possible
	 */

	auto data       = std::vector<std::vector<T>>{};
	auto str_val_fn = str_fn<T>();

	data.reserve(rows);

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto val_str = std::string{};
		auto stream  = std::istringstream{ line };
		auto row     = std::vector<T>{};

		row.reserve(columns);

		while (std::getline(stream, val_str, ' '))
		{
			row.push_back(str_val_fn(val_str));
		}

		data.push_back(std::move(row));
	}

	return fn.template operator()<T>(rows, columns, std::move(data));
}

template<typename T>
auto parse_mat_with_dims(std::ifstream& file, std::string& line, const auto& fn)
{
	std::size_t rows, columns;
	std::getline(file, line);

	auto stream = std::istringstream{ line };
	stream >> rows >> columns;

	return parse_mat_elems<T>(file, line, rows, columns, fn);
}

template<typename From, typename To>
struct num_out
{
	mpp::matrix<From> mat;
	To num;
};

template<typename From, typename To>
auto parse_num_out(const std::filesystem::path& path) -> num_out<From, To>
{
	auto data_file  = std::ifstream{ path };
	auto line       = std::string{};
	auto num_val_fn = str_fn<To>();

	auto mat = parse_mat_with_dims<From>(data_file, line, mat_fn);

	std::getline(data_file, line);
	const auto out = num_val_fn(line);

	return { std::move(mat), out };
}

template<typename To>
struct block
{
	std::size_t row_start;
	std::size_t column_start;
	std::size_t row_end;
	std::size_t column_end;
	mpp::matrix<To> mat;
};

template<typename From, typename To>
struct block_out
{
	mpp::matrix<From> mat;
	std::vector<block<To>> blocks;
};

template<typename From, typename To>
auto parse_block_out(const std::filesystem::path& path) -> block_out<From, To>
{
	auto data_file = std::ifstream{ path };
	auto line      = std::string{};
	auto blocks    = std::vector<block<To>>{};

	auto mat = parse_mat_with_dims<From>(data_file, line, mat_fn);
	std::size_t row_start, column_start, row_end, column_end;

	while (std::getline(data_file, line))
	{
		// Parse the block indices
		{
			auto line_stream = std::istringstream{ line };
			line_stream >> row_start >> column_start >> row_end >> column_end;
		}

		// Parse the block
		auto block_ =
			parse_mat_elems<To>(data_file, line, row_end - row_start + 1, column_end - column_start + 1, mat_fn);
		blocks.push_back(block<To>{ row_start, column_start, row_end, column_end, block_ });
	}

	return { std::move(mat), std::move(blocks) };
}

template<std::size_t I, std::size_t Size, template<typename> typename... TempTs, typename... ValTs, typename... Fns>
auto parse_mats_out_impl(std::ifstream& file,
	std::string& line,
	auto& out,
	const std::tuple<Fns...>& fns,
	temp_types<TempTs...> temp_ts,
	types<ValTs...> val_ts)
{
	if constexpr (I + 1 > Size)
	{
		return out;
	}
	else
	{
		using val_t = std::tuple_element_t<I, std::tuple<ValTs...>>;

		std::get<I>(out) = parse_mat_with_dims<val_t>(file, line, std::get<I>(fns));

		return parse_mats_out_impl<I + 1, Size>(file, line, out, fns, temp_ts, val_ts);
	}
}

template<template<typename> typename... TempTs2, typename... ValTs2>
auto create_corresponding_tup(temp_types<TempTs2...>, types<ValTs2...>)
{
	return std::tuple<TempTs2<ValTs2>...>{};
}

template<typename TempTs, typename ValTs, typename... Fns>
auto parse_mats_out(const std::filesystem::path& path, const std::tuple<Fns...>& fns)
{
	auto out = create_corresponding_tup(TempTs{}, ValTs{});

	auto line = std::string{};
	auto file = std::ifstream(path);

	return parse_mats_out_impl<0, TempTs::size>(file, line, out, fns, TempTs{}, ValTs{});
}
