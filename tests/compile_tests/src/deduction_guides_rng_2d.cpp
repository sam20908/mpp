// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=deduction_guides_rng_2d -DTEST_SOURCE=%s -B build/deduction_guides_rng_2d
// RUN: cmake --build build/deduction_guides_rng_2d --target deduction_guides_rng_2d

#include <matrixpp/matrix.hpp>
#include <vector>

int main()
{
	auto rng_2d = std::vector{ std::vector{ 1, 2, 3 }, std::vector{ 1, 2, 3 } };
	(void)matrixpp::matrix{ rng_2d };

	return 0;
}