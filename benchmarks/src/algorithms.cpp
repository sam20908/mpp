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
#include <cstddef>
#include <matrixpp/algorithm.hpp>
#include <matrixpp/matrix/fully_dynamic.hpp>
#include <utility>

static void Determinant(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		// Use long double to ensure no overflow or underflow
		(void)matrixpp::determinant(std::type_identity<long double>{}, matrix);
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Inverse(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 125 };

	for (auto _ : state)
	{
		// Use long double to ensure no overflow or underflow
		(void)matrixpp::inverse(std::type_identity<long double>{}, matrix);
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Block(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ 1024, 1024, 0 };

	for (auto _ : state)
	{
		// -1 is for avoiding out of range on reaching max size
		auto n = state.range() == 0 ? 0 : state.range() - 1;

		(void)matrixpp::block(matrix, 0, 0, n, n);
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

static void Transpose(benchmark::State& state)
{
	auto matrix = matrixpp::matrix<int>{ state.range(), state.range(), 0 };

	for (auto _ : state)
	{
		(void)matrixpp::transpose(matrix);
	}

	state.counters["Rows"]    = state.range();
	state.counters["Columns"] = state.range();
}

BENCHMARK(Determinant)->DenseRange(0, 15, 1);
BENCHMARK(Inverse)->DenseRange(0, 15, 1);
BENCHMARK(Block)->DenseRange(0, 1024, 128);
BENCHMARK(Transpose)->DenseRange(0, 1024, 128);