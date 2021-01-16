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
#include <matrixpp/algorithm/determinant.hpp>
#include <matrixpp/matrix.hpp>
#include <utility>

static void determinant_5x5(benchmark::State& state)
{
	auto a = matrixpp::matrix<int, 5, 5>{ 125 };

	for (auto _ : state)
	{
		(void)matrixpp::determinant(std::type_identity<long double>{}, a);
	}
}

static void determinant_10x10(benchmark::State& state)
{
	auto a = matrixpp::matrix<int, 10, 10>{ 125 };

	for (auto _ : state)
	{
		(void)matrixpp::determinant(std::type_identity<long double>{}, a);
	}
}

static void determinant_100x100(benchmark::State& state)
{
	auto a = matrixpp::matrix<int, 100, 100>{ 125 };

	for (auto _ : state)
	{
		(void)matrixpp::determinant(std::type_identity<long double>{}, a);
	}
}

BENCHMARK(determinant_5x5);
BENCHMARK(determinant_10x10);
// BENCHMARK(determinant_100x100);