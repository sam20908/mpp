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
	void insert_expr_content_into_out_stream(auto& out, const auto& expr)
	{
		const auto rows    = expr.rows();
		const auto columns = expr.columns();

		for (auto row = std::size_t{}; row < rows; ++row)
		{
			for (auto column = std::size_t{}; column < columns; ++column)
			{
				const auto value = expr(row, column);
				out << value << (column == columns - 1 ? '\n' : ' ');
			}
		}
	}
} // namespace mpp::detail
