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
auto parse_mat_elems(auto& data_file, auto& line, std::size_t rows, std::size_t columns) -> mpp::matrix<T>
{
	auto data          = std::vector<T>{};
	auto from_value_fn = str_fn<T>();

	data.reserve(rows * columns);

	while (std::getline(data_file, line))
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
auto parse_mat_with_dims(auto& data_file, auto& line) -> mpp::matrix<T>
{
	std::size_t rows, columns;
	std::getline(data_file, line);

	auto line_stream = std::istringstream{ line };
	line_stream >> rows >> columns;

	return parse_mat_elems<T>(data_file, line, rows, columns);
}

template<typename From, typename To>
struct num_out
{
	mpp::matrix<From> original_data;
	To result;
	bool file_exists;
};

template<typename From, typename To>
auto parse_num_out(const std::filesystem::path& data_file_path) -> num_out<From, To>
{
	auto data_file       = std::ifstream{ data_file_path };
	auto line            = std::string{};
	auto result_value_fn = str_fn<To>();

	auto matrix = parse_mat_with_dims<From>(data_file, line);

	std::getline(data_file, line);
	const auto result = result_value_fn(line);

	return { std::move(matrix), result, data_file.is_open() };
}

template<typename From, typename To>
struct mat_out
{
	mpp::matrix<From> original_matrix;
	mpp::matrix<To> transformed_matrix;
	bool file_exists;
};

template<typename From, typename To>
auto parse_mat_out(const std::filesystem::path& data_file_path) -> mat_out<From, To>
{
	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	auto from = parse_mat_with_dims<From>(data_file, line);
	auto to   = parse_mat_with_dims<To>(data_file, line);

	return { std::move(from), std::move(to), data_file.is_open() };
}

template<typename To>
struct block
{
	std::size_t row_start;
	std::size_t column_start;
	std::size_t row_end;
	std::size_t column_end;
	mpp::matrix<To> block_matrix;
};

template<typename From, typename To>
struct block_out
{
	mpp::matrix<From> original_matrix;
	std::vector<block<To>> blocks;
	bool file_exists;
};

template<typename From, typename To>
auto parse_block_out(const std::filesystem::path& data_file_path) -> block_out<From, To>
{
	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};
	auto blocks    = std::vector<block<To>>{};

	auto from = parse_mat_with_dims<From>(data_file, line);
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

	return { std::move(from), std::move(blocks), data_file.is_open() };
}

template<typename From, typename To>
struct two_mat_out
{
	mpp::matrix<From> original_matrix;
	mpp::matrix<To> l_matrix;
	mpp::matrix<To> u_matrix;
};

template<typename From, typename To>
auto parse_two_mat_out(const std::filesystem::path& data_file_path) -> two_mat_out<From, To>
{
	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	auto from = parse_mat_with_dims<From>(data_file, line);
	auto l    = parse_mat_with_dims<To>(data_file, line);
	auto u    = parse_mat_with_dims<To>(data_file, line);

	return { std::move(from), std::move(l), std::move(u) };
}

template<typename AValue, typename XValue, typename BValue>
struct subst_out
{
	mpp::matrix<AValue> a;
	mpp::matrix<XValue> x;
	mpp::matrix<BValue> b;
};

template<typename AValue, typename XValue, typename BValue>
auto parse_subst_out(const std::filesystem::path& data_file_path) -> subst_out<AValue, XValue, BValue>
{
	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	auto a = parse_mat_with_dims<AValue>(data_file, line);
	auto b = parse_mat_with_dims<BValue>(data_file, line);
	auto x = parse_mat_with_dims<XValue>(data_file, line);

	return { std::move(a), std::move(x), std::move(b) };
}
