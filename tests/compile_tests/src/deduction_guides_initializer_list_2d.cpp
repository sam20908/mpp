// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=deduction_guides_initializer_list_2d -DTEST_SOURCE=%s -B build/deduction_guides_initializer_list_2d
// RUN: cmake --build build/deduction_guides_initializer_list_2d --target deduction_guides_initializer_list_2d

#include <matrixpp/matrix.hpp>

int main()
{
	(void)matrixpp::matrix{ { { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } } };

	return 0;
}