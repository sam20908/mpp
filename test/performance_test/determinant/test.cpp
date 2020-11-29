#include <benchmark/benchmark.h>
#include <matrixpp/matrixpp.hpp>

static void determinant_5x5(benchmark::State& state)
{
	for (auto _ : state)
	{
		matrixpp::matrix a{ { 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 },
			{ 1000, 1000, 100, 10, 1 } };

		(void)a.determinant();
	}
}

// static void determinant_100x100(benchmark::State& state)
// {
// 	for (auto _ : state)
// 	{
// 		matrixpp::matrix a(100, 100, 12345);

// 		(void)a.determinant<long double>();
// 	}
// }

BENCHMARK(determinant_5x5);
// BENCHMARK(determinant_100x100);