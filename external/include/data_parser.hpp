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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template<typename T>
inline auto string_conversion_function()
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

template<typename From, typename To>
struct single_number_result
{
	mpp::matrix<From> original_data;
	To result;
	bool file_exists;
};

template<typename From, typename To>
inline auto parse_data_file_with_single_number_result(const std::filesystem::path& data_file_path)
	-> single_number_result<From, To>
{
	const auto from_value_conversion_function   = string_conversion_function<From>();
	const auto result_value_conversion_function = string_conversion_function<To>();

	auto original_data = std::vector<std::vector<From>>{};
	std::size_t original_data_rows, original_data_columns;

	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	// Parse the dimensions
	{
		std::getline(data_file, line);

		auto line_stream = std::istringstream{ line };
		line_stream >> original_data_rows >> original_data_columns;

		original_data.reserve(original_data_rows);
	}

	// Parse the original matrix
	while (std::getline(data_file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto line_stream = std::istringstream{ line };
		auto row         = std::vector<From>{};

		row.reserve(original_data_columns);

		auto value_string = std::string{};
		while (std::getline(line_stream, value_string, ' '))
		{
			auto data_num = from_value_conversion_function(value_string);
			row.push_back(data_num);
		}

		original_data.push_back(std::move(row));
	}

	// Parse the result
	std::getline(data_file, line);
	const auto result = result_value_conversion_function(line);

	return { mpp::matrix{ original_data }, result, data_file.is_open() };
}

template<typename From, typename To>
struct transformation_result
{
	mpp::matrix<From> original_matrix;
	mpp::matrix<To> transformed_matrix;
	bool file_exists;
};

template<typename From, typename To>
inline auto parse_data_file_matrix_transformation(const std::filesystem::path& data_file_path)
	-> transformation_result<From, To>
{
	const auto from_value_conversion_function   = string_conversion_function<From>();
	const auto result_value_conversion_function = string_conversion_function<To>();

	auto original_data    = std::vector<std::vector<From>>{};
	auto transformed_data = std::vector<std::vector<To>>{};
	std::size_t original_data_rows, original_data_columns, transformed_data_rows, transformed_data_columns;

	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	// Parse the original matrix dimensions
	{
		std::getline(data_file, line);

		auto line_stream = std::istringstream{ line };
		line_stream >> original_data_rows >> original_data_columns;

		original_data.reserve(original_data_rows);
	}

	// Parse the original matrix
	while (std::getline(data_file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto value_string = std::string{};
		auto line_stream  = std::istringstream{ line };
		auto row          = std::vector<From>{};

		row.reserve(original_data_columns);

		while (std::getline(line_stream, value_string, ' '))
		{
			const auto data_num = from_value_conversion_function(value_string);
			row.push_back(data_num);
		}

		original_data.push_back(std::move(row));
	}

	// Parse the transformed matrix dimensions
	{
		std::getline(data_file, line);

		auto line_stream = std::istringstream{ line };
		line_stream >> transformed_data_rows >> transformed_data_columns;

		transformed_data.reserve(transformed_data_rows);
	}

	// Parse the transformed matrix
	while (std::getline(data_file, line))
	{
		auto value_string = std::string{};
		auto line_stream  = std::istringstream{ line };
		auto row          = std::vector<To>{};

		row.reserve(transformed_data_columns);

		while (std::getline(line_stream, value_string, ' '))
		{
			const auto data_num = result_value_conversion_function(value_string);
			row.push_back(data_num);
		}

		transformed_data.push_back(std::move(row));
	}

	return { mpp::matrix{ original_data }, mpp::matrix{ transformed_data }, data_file.is_open() };
}

template<typename To>
struct block_transformation_testcase
{
	std::size_t row_start;
	std::size_t column_start;
	std::size_t row_end;
	std::size_t column_end;
	mpp::matrix<To> block_matrix;
};

template<typename From, typename To>
struct block_transformation_result
{
	mpp::matrix<From> original_matrix;
	std::vector<block_transformation_testcase<To>> blocks;
	bool file_exists;
};

template<typename From, typename To>
inline auto parse_data_file_block_transformation(const std::filesystem::path& data_file_path)
	-> block_transformation_result<From, To>
{
	const auto from_value_conversion_function   = string_conversion_function<From>();
	const auto result_value_conversion_function = string_conversion_function<To>();

	auto original_data = std::vector<std::vector<From>>{};
	auto blocks        = std::vector<block_transformation_testcase<To>>{};
	std::size_t original_data_rows, original_data_columns;

	auto data_file = std::ifstream{ data_file_path };
	auto line      = std::string{};

	// Parse the original matrix dimensions
	{
		std::getline(data_file, line);

		auto line_stream = std::istringstream{ line };
		line_stream >> original_data_rows >> original_data_columns;

		original_data.reserve(original_data_rows);
	}

	// Parse the original matrix
	while (std::getline(data_file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto value_string = std::string{};
		auto line_stream  = std::istringstream{ line };
		auto row          = std::vector<From>{};

		row.reserve(original_data_columns);

		while (std::getline(line_stream, value_string, ' '))
		{
			const auto data_num = from_value_conversion_function(value_string);
			row.push_back(data_num);
		}

		original_data.push_back(std::move(row));
	}

	// Parse each block testcase
	{
		auto line_is_block_indices = true;
		auto current_block_data    = std::vector<std::vector<To>>{};
		std::size_t row_start{}, column_start{}, row_end{}, column_end{};

		while (std::getline(data_file, line))
		{
			if (line == "=")
			{
				blocks.emplace_back(row_start, column_start, row_end, column_end, mpp::matrix{ current_block_data });
				line_is_block_indices = true;
				current_block_data.clear();
				continue;
			}

			auto line_stream = std::istringstream{ line };

			if (line_is_block_indices)
			{
				line_stream >> row_start >> column_start >> row_end >> column_end;
				line_is_block_indices = false;

				current_block_data.reserve(row_end - row_start);
				continue;
			}

			auto row          = std::vector<To>{};
			auto value_string = std::string{};

			row.reserve(column_end - column_start);

			while (std::getline(line_stream, value_string, ' '))
			{
				const auto value = result_value_conversion_function(value_string);
				row.push_back(value);
			}

			current_block_data.push_back(std::move(row));
		}
	}

	return { mpp::matrix{ std::move(original_data) }, std::move(blocks), data_file.is_open() };
}