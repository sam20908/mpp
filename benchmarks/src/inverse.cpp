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

#include <benchmark/benchmark.h>
#include <matrixpp/algorithm/inverse.hpp>
#include <matrixpp/matrix.hpp>
#include <utility>

static void inverse_5x5(benchmark::State& state)
{
	auto a = matrixpp::matrix{ { 6, 3, 5, 7, 2 },
		{ 8, 4, 0, 1, 2 },
		{ 6, 4, 8, 3, 4 },
		{ -2, 3, 5, 2, 1 },
		{ -7, -7, -5, 1, 2 } };

	for (auto _ : state)
	{
		(void)matrixpp::inverse(std::type_identity<long double>{}, a);
	}
}

static void inverse_10x10(benchmark::State& state)
{
	auto a = matrixpp::matrix{ { 6, 3, 5, 7, 2, 1, 5, 2, 3, 6 },
		{ 8, 4, 0, 1, 2, 8, 5, 6, 1, 1 },
		{ 6, 4, 8, 3, 4, -9, 7, 8, 3, -1 },
		{ -2, 3, 5, 2, 1, 2, 3, 4, 5, 6 },
		{ -7, -7, -5, 1, 2, 2, 3, 3, 6, 7 },
		{ -7, -5, -5, 1, 2, 2, 4, 5, 6, 7 },
		{ -7, 1, -5, 1, -2, 2, 31, 5, 6, 7 },
		{ -7, 9, -5, 1, 2, 2, 3, 5, 6, 1 },
		{ -7, -7, -5, 1, 2, 22, 3, 9, 2, 7 },
		{ -7, -5, -5, 1, 2, 2, 3, 5, 6, 7 } };

	for (auto _ : state)
	{
		(void)matrixpp::inverse(std::type_identity<long double>{}, a);
	}
}

static void inverse_100x100(benchmark::State& state)
{
	// @TODO: Use random utility method
	auto a = matrixpp::matrix<int>(1, 1, 200); // @TODO: Use actual data

	for (auto _ : state)
	{
		(void)matrixpp::inverse(std::type_identity<long double>{}, a);
	}
}

BENCHMARK(inverse_5x5);
BENCHMARK(inverse_10x10);
// BENCHMARK(inverse_100x100);