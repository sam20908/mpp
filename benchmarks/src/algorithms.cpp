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

#include <algorithm>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <matrixpp/algorithm.hpp>
#include <matrixpp/matrix/fully_dynamic.hpp>
#include <matrixpp/utility/singular.hpp>
#include <random>
#include <utility>

namespace
{
	// Benchmark limit without hanging (UB?)
	static constexpr auto MAX_DETERMINANT_NXN_WITHOUT_BREAKING = 13;
	static constexpr auto MAX_INVERSE_NXN_WITHOUT_BREAKING     = 11;

	static inline auto random_device       = std::random_device{};
	static inline auto random_engine       = std::mt19937{ random_device() };
	static inline auto random_distribution = std::uniform_int_distribution{ 1, 50 };

	void fill_matrix_with_random_values(auto& matrix)
	{
		std::ranges::transform(matrix, matrix.begin(), [](auto) {
			return random_distribution(random_engine);
		});
	}
} // namespace

static void Determinant(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(matrix);
		// Use long double to ensure no overflow or underflow
		benchmark::DoNotOptimize(matrixpp::determinant(std::type_identity<long double>{}, matrix));
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Inverse(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 0 };
	fill_matrix_with_random_values(matrix); // Initial fill, possibly non-singular first try

	while (matrixpp::singular(matrix))
	{
		fill_matrix_with_random_values(matrix);
	}

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(matrix);
		// Use long double to ensure no overflow or underflow
		benchmark::DoNotOptimize(matrixpp::inverse(std::type_identity<long double>{}, matrix));
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Block(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 0 };
	auto n      = state.range() == 0 ? 0 : state.range() - 1; // avoid out of range on reaching max size

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(matrix);
		benchmark::DoNotOptimize(n);
		benchmark::DoNotOptimize(matrixpp::block(matrix, 0, 0, n, n));
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Transpose(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 0 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(matrix);
		benchmark::DoNotOptimize(matrixpp::transpose(matrix));
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

BENCHMARK(Determinant)->DenseRange(0, MAX_DETERMINANT_NXN_WITHOUT_BREAKING, 1);
BENCHMARK(Inverse)->DenseRange(0, MAX_INVERSE_NXN_WITHOUT_BREAKING, 1);
BENCHMARK(Block)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Transpose)->RangeMultiplier(2)->Range(8, 8 << 10);