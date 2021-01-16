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

// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=customize_default_extents -DTEST_SOURCE=%s -B build/customize_default_extents
// RUN: cmake --build build/customize_default_extents --target customize_default_extents

#include <matrixpp/utility/config.hpp>

namespace matrixpp::customize
{
	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_rows_extent_tag, customize_tag)
	{
		return 10;
	}

	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_columns_extent_tag, customize_tag)
	{
		return 10;
	}
} // namespace matrixpp::customize

#include <cassert>
#include <matrixpp/matrix.hpp>

int main()
{
	auto m = matrixpp::matrix<int>{};
	assert(m.rows_extent() == 10);
	assert(m.columns_extent() == 10);

	return 0;
}