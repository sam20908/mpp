// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=1d_range_constructor -DTEST_SOURCE=%s -B build/1d_range_constructor
// RUN: cmake --build build/1d_range_constructor --target 1d_range_constructor
// XFAIL: *

#include <array>
#include <matrixpp/matrix.hpp>

int main()
{
	std::array rng_1d{ 1, 2, 3, 4, 5, 6 };
	(void)matrixpp::matrix<int>{ rng_1d };

	return 0;
}