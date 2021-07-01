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

#include <cstddef>

namespace mpp::detail
{
	void insert_expr_content_into_out_stream(auto& out, const auto& expr, const char* end)
	{
		const auto rows = expr.rows();
		const auto cols = expr.columns();

		if (rows == 0 && cols == 0)
		{
			out << "[empty matrix]\n";
		}
		else
		{
			for (auto row = std::size_t{}; row < rows; ++row)
			{
				for (auto col = std::size_t{}; col < cols; ++col)
				{
					const auto value = expr(row, col);
					out << value;

					if (col == cols - 1)
					{
						if (row == rows - 1)
						{
							out << end;
						}
						else
						{
							out << '\n';
						}
					}
					else
					{
						out << ' ';
					}
				}
			}
		}
	}
} // namespace mpp::detail
