// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=2d_range_constructor -DTEST_SOURCE=%s -B build/2d_range_constructor
// RUN: cmake --build build/2d_range_constructor --target 2d_range_constructor

#include <array>
#include <matrixpp/matrix.hpp>

int main()
{
	auto rng_2d = std::array{ std::array{ 1, 2, 3 }, std::array{ 1, 2, 3 } };
	(void)matrixpp::matrix<int>{ rng_2d };

	return 0;
}