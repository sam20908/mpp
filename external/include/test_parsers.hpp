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

template<typename T>
auto parse_mat_elems(std::ifstream& file, std::string& line, std::size_t rows, std::size_t columns) -> mpp::matrix<T>
{
	auto data          = std::vector<T>{};
	auto from_value_fn = str_fn<T>();

	data.reserve(rows * columns);

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto value_string = std::string{};
		auto line_stream  = std::istringstream{ line };

		while (std::getline(line_stream, value_string, ' '))
		{
			data.push_back(from_value_fn(value_string));
		}
	}

	return mpp::matrix<T>{ rows, columns, std::move(data) };
}

template<typename T>
auto parse_mat_with_dims(std::ifstream& file, std::string& line) -> mpp::matrix<T>
{
	std::size_t rows, columns;
	std::getline(file, line);

	auto line_stream = std::istringstream{ line };
	line_stream >> rows >> columns;

	return parse_mat_elems<T>(file, line, rows, columns);
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

	auto mat = parse_mat_with_dims<From>(data_file, line);

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

	auto mat = parse_mat_with_dims<From>(data_file, line);
	std::size_t row_start, column_start, row_end, column_end;

	while (std::getline(data_file, line))
	{
		// Parse the block indices
		{
			auto line_stream = std::istringstream{ line };
			line_stream >> row_start >> column_start >> row_end >> column_end;
		}

		// Parse the block
		auto block_ = parse_mat_elems<To>(data_file, line, row_end - row_start + 1, column_end - column_start + 1);
		blocks.push_back(block<To>{ row_start, column_start, row_end, column_end, block_ });
	}

	return { std::move(mat), std::move(blocks) };
}

template<typename... Ts>
using mats_out = std::tuple<mpp::matrix<Ts>...>;

template<std::size_t I, std::size_t Size, typename Tup>
auto parse_mats_out_impl(std::ifstream& file, std::string& line, Tup& out)
{
	if constexpr (I == Size)
	{
		return out;
	}
	else
	{
		using mat_val_t = typename std::tuple_element_t<I, Tup>::value_type;

		std::get<I>(out) = parse_mat_with_dims<mat_val_t>(file, line);

		return parse_mats_out_impl<I + 1, Size>(file, line, out);
	}
}

template<typename... Ts>
auto parse_mats_out(const std::filesystem::path& path) -> mats_out<Ts...>
{
	auto out  = mats_out<Ts...>{};
	auto line = std::string{};
	auto file = std::ifstream(path);

	return parse_mats_out_impl<0, sizeof...(Ts)>(file, line, out);
}
