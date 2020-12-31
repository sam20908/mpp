// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=3d_range_constructor -DTEST_SOURCE=%s -B build/3d_range_constructor
// RUN: cmake --build build/3d_range_constructor --target 3d_range_constructor
// XFAIL: *

#include <array>
#include <matrixpp/matrix.hpp>

int main()
{
	std::array<std::array<std::array<int, 1>, 1>, 1> rng_3d{ std::array{ std::array{ 1 } } };
	(void)matrixpp::matrix<int>{ rng_3d };

	return 0;
}