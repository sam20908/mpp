// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=deduction_guides_expr_template -DTEST_SOURCE=%s -B build/deduction_guides_expr_template
// RUN: cmake --build build/deduction_guides_expr_template --target deduction_guides_expr_template

#include <matrixpp/matrix.hpp>
#include <matrixpp/operation/add.hpp>

int main()
{
	auto matrix = matrixpp::matrix<int, 3, 3>{ { 7, 3, 1 }, { 8, 8, 2 }, { 5, 8, 2 } };
	(void)matrixpp::matrix{ matrix + matrix };

	return 0;
}