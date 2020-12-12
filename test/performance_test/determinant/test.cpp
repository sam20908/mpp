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
#include <matrixpp/matrixpp.hpp>

static void determinant_5x5(benchmark::State& state)
{
	matrixpp::matrix a(5, 5, 125);

	for (auto _ : state)
	{
		(void)a.determinant();
	}
}

static void determinant_10x10(benchmark::State& state)
{
	matrixpp::matrix a(10, 10, 125);

	for (auto _ : state)
	{
		(void)a.determinant();
	}
}

static void determinant_100x100(benchmark::State& state)
{
	matrixpp::matrix a(100, 100, 1);

	for (auto _ : state)
	{
		(void)a.determinant<long double>();
	}
}

BENCHMARK(determinant_5x5);
BENCHMARK(determinant_10x10);
// BENCHMARK(determinant_100x100);