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

#include <mpp/matrix.hpp>

#include "../../include/ut_benchmark.hpp"

#include <span>
#include <string_view>

template<std::size_t RowsExtent, std::size_t ColumnsExtent>
void benchmark_initialization(std::string_view name, auto... args)
{
    // Using scope resolution operator to workaround ambiguity
	benchmark::benchmark{ name } = [&]() {
		auto matrix = mpp::matrix<int, RowsExtent, ColumnsExtent>{ args... };
		benchmark::do_not_optimize(matrix);
	};
}

// @TODO: Port rest of benchmarks (#174)

int main()
{

	/**
	 * Note that for some reason, the benchmark example from Boost.UT is different from Google.Benchmark results (it
	 * takes my computer 100ns to create an integer according to this LOL)
	 */

	// Not giving static matrices big extents because the stack is very small
	benchmark_initialization<10, 10>("default_initialization_fully_static_10x10");
	benchmark_initialization<std::dynamic_extent, std::dynamic_extent>("default_initialization_fully_dynamic_10x10",
		10ul,
		10ul);
	benchmark_initialization<std::dynamic_extent, 10>("default_initialization_dynamic_rows_10x10", 10ul);
	benchmark_initialization<10, std::dynamic_extent>("default_initialization_dynamic_columns_10x10", 10ul);

	return 0;
}
