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

#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

inline auto parse_data_file_double_det(const std::string& filename)
	-> std::tuple<std::vector<std::vector<double>>, double, bool>
{
	auto file   = std::ifstream{ filename };
	auto data   = std::vector<std::vector<double>>{};
	auto result = double{};

	auto line           = std::string{};
	auto line_is_result = false;

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			line_is_result = true;
			continue;
		}

		if (line_is_result)
		{
			result = std::stod(line);
		}
		else
		{
			auto data_token  = std::string{};
			auto line_stream = std::istringstream{ line };
			auto row         = std::vector<double>{};

			while (std::getline(line_stream, data_token, ' '))
			{
				auto data_num = std::stod(data_token);
				row.push_back(data_num);
			}

			data.push_back(std::move(row));
		}
	}

	return { data, result, file.is_open() };
}

inline auto parse_data_file_matrix_transformation_double(const std::string& filename)
	-> std::tuple<std::vector<std::vector<double>>, std::vector<std::vector<double>>, bool>
{
	auto file   = std::ifstream{ filename };
	auto data   = std::vector<std::vector<double>>{};
	auto result = std::vector<std::vector<double>>{};

	auto line           = std::string{};
	auto line_is_result = false;

	while (std::getline(file, line))
	{
		if (line == "=")
		{
			line_is_result = true;
			continue;
		}

		if (line_is_result)
		{
			auto data_token  = std::string{};
			auto line_stream = std::istringstream{ line };
			auto row         = std::vector<double>{};

			while (std::getline(line_stream, data_token, ' '))
			{
				auto data_num = std::stod(data_token);
				row.push_back(data_num);
			}

			result.push_back(std::move(row));
		}
		else
		{
			auto data_token  = std::string{};
			auto line_stream = std::istringstream{ line };
			auto row         = std::vector<double>{};

			while (std::getline(line_stream, data_token, ' '))
			{
				auto data_num = std::stod(data_token);
				row.push_back(data_num);
			}

			data.push_back(std::move(row));
		}
	}

	return { data, result, file.is_open() };
}

inline auto parse_data_file_matrix_block_double(const std::string& filename)
	-> std::tuple<std::vector<std::vector<double>>,
		std::vector<std::tuple<std::size_t, std::size_t, std::size_t, std::size_t, std::vector<std::vector<double>>>>,
		bool>
{
	auto file = std::ifstream{ filename };
	auto data = std::vector<std::vector<double>>{};
	auto results =
		std::vector<std::tuple<std::size_t, std::size_t, std::size_t, std::size_t, std::vector<std::vector<double>>>>{};
	auto testcase = std::tuple<std::size_t, std::size_t, std::size_t, std::size_t, std::vector<std::vector<double>>>{};
	auto block    = std::vector<std::vector<double>>{};

	auto line = std::string{};

	// Parse the data first
	while (std::getline(file, line))
	{
		if (line == "=")
		{
			break;
		}

		auto data_token  = std::string{};
		auto line_stream = std::istringstream{ line };
		auto row         = std::vector<double>{};

		while (std::getline(line_stream, data_token, ' '))
		{
			auto data_num = std::stod(data_token);
			row.push_back(data_num);
		}

		data.push_back(std::move(row));
	}

	auto line_is_block_testcase_data = false;
	// Helper variable to determine whether we need to manually push the last read test case
	auto has_test_case = false; // Parse the test cases

	while (std::getline(file, line))
	{
		has_test_case = true;

		if (line == "=")
		{
			std::get<4>(testcase) = std::move(block);
			results.push_back(std::move(testcase));
			line_is_block_testcase_data = false;
		}
		else
		{
			auto data_token  = std::string{};
			auto line_stream = std::istringstream{ line };

			if (!line_is_block_testcase_data)
			{
				std::size_t row_start, column_start, row_end, column_end;
				line_stream >> row_start >> column_start >> row_end >> column_end;

				std::get<0>(testcase) = row_start;
				std::get<1>(testcase) = column_start;
				std::get<2>(testcase) = row_end;
				std::get<3>(testcase) = column_end;

				line_is_block_testcase_data = true;
			}
			else
			{
				auto row = std::vector<double>{};

				while (std::getline(line_stream, data_token, ' '))
				{
					auto data_num = std::stod(data_token);
					row.push_back(data_num);
				}

				block.push_back(std::move(row));
			}
		}
	}

	if (has_test_case)
	{
		// The last testcase isn't being pushed to results because the while loop will run out of lines to parse, so we
		// have to push it manually
		std::get<4>(testcase) = std::move(block);
		results.push_back(std::move(testcase));
	}

	return { data, results, file.is_open() };
}
