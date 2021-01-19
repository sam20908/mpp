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
#include <matrixpp/matrix/fully_dynamic.hpp>
#include <matrixpp/operation.hpp>

static void Addition(benchmark::State& state)
{
	auto a = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto b = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		(void)matrixpp::matrix{ a + b };
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Addition_CompoundAssign(benchmark::State& state)
{
	auto a = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto b = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		a += b;
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Subtraction(benchmark::State& state)
{
	auto a = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto b = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		(void)matrixpp::matrix{ a - b };
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Subtraction_CompoundAssign(benchmark::State& state)
{
	auto a = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto b = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		a -= b;
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Multiplication_Scalar(benchmark::State& state)
{
	auto a      = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto scalar = 1;

	for (auto _ : state)
	{
		(void)matrixpp::matrix{ a * scalar };
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Multiplication_Scalar_CompoundAssign(benchmark::State& state)
{
	auto a      = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto scalar = 1;

	for (auto _ : state)
	{
		a *= scalar;
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Division_Scalar(benchmark::State& state)
{
	auto a      = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto scalar = 1;

	for (auto _ : state)
	{
		(void)matrixpp::matrix{ a / scalar };
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Division_Scalar_CompoundAssign(benchmark::State& state)
{
	auto a      = matrixpp::matrix<int>{ state.range(), state.range(), 125 };
  auto scalar = 1;

	for (auto _ : state)
	{
		a /= scalar;
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

// @TODO: Add multiplication x scalar benchmark

BENCHMARK(Addition)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Addition_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Subtraction)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Subtraction_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Multiplication_Scalar)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Multiplication_Scalar_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Division_Scalar)->RangeMultiplier(2)->Range(8, 8 << 10);
BENCHMARK(Division_Scalar_CompoundAssign)->RangeMultiplier(2)->Range(8, 8 << 10);
