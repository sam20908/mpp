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

#include <mpp/matrix/fully_dynamic.hpp>
#include <mpp/arithmetic.hpp>

#include <algorithm>
#include <cstddef>

static void Addition(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(mpp::matrix{ a + b });
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Addition_CompoundAssign(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(a += b);
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Subtraction(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(mpp::matrix{ a - b });
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Subtraction_CompoundAssign(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(a -= b);
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Multiplication_Scalar(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto scalar = 1;

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(scalar);
		benchmark::DoNotOptimize(mpp::matrix{ a * scalar });
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Multiplication_Scalar_CompoundAssign(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto scalar = 1;

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(scalar);
		benchmark::DoNotOptimize(a *= scalar);
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Division_Scalar(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto scalar = 1;

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(scalar);
		benchmark::DoNotOptimize(mpp::matrix{ a / scalar });
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Division_Scalar_CompoundAssign(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto scalar = 1;

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(scalar);
		benchmark::DoNotOptimize(a /= scalar);
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Multiplication_Matrix(benchmark::State& state)
{
	auto a =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		mpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(mpp::matrix{ a * b });
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

BENCHMARK(Addition)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Addition_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Subtraction)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Subtraction_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Multiplication_Scalar)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Multiplication_Scalar_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Multiplication_Matrix)->DenseRange(0, 1024, 128);
BENCHMARK(Division_Scalar)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Division_Scalar_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
