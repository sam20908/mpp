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