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

#include <matrixpp/matrix/fully_dynamic.hpp>
#include <matrixpp/utility.hpp>

// Don't benchmark singular because determinant is the only heavy operation

static void Cast(benchmark::State& state)
{
	auto a =
		matrixpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(matrixpp::cast(std::type_identity<long double>{}, a));
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

static void Equal(benchmark::State& state)
{
	auto a =
		matrixpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };
	auto b =
		matrixpp::matrix<int>{ static_cast<std::size_t>(state.range()), static_cast<std::size_t>(state.range()), 125 };

	benchmark::ClobberMemory();
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(a);
		benchmark::DoNotOptimize(b);
		benchmark::DoNotOptimize(a == b);
		benchmark::ClobberMemory();
	}

	state.counters["Rows"]    = static_cast<double>(state.range());
	state.counters["Columns"] = static_cast<double>(state.range());
}

BENCHMARK(Cast)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Equal)->RangeMultiplier(2)->Range(8, 8 << 10);