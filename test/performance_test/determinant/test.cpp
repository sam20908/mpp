#include <benchmark/benchmark.h>
#include <matrixpp/matrixpp.hpp>

static void determinant_5x5(benchmark::State& state)
{
	matrixpp::matrix a{ { 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 } };

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
		// @TODO: Fix this when determinant API changes
		(void)a.determinant<long double>();
	}
}

BENCHMARK(determinant_5x5);
// BENCHMARK(determinant_100x100);