// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=one_side_zero_and_other_non_zero -DTEST_SOURCE=%s -B build/one_side_zero_and_other_non_zero
// RUN: cmake --build build/one_side_zero_and_other_non_zero --target one_side_zero_and_other_non_zero
// XFAIL: *

#include <matrixpp/matrix.hpp>

int main()
{
	(void)matrixpp::matrix<int, 0, 1>{};

	return 0;
}