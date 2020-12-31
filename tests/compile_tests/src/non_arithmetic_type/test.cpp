// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=non_arithmetic_type -DTEST_SOURCE=%s -B build/non_arithmetic_type
// RUN: cmake --build build/non_arithmetic_type --target non_arithmetic_type
// XFAIL: *

#include <matrixpp/matrix.hpp>

struct non_arithmetic_type
{
};

int main()
{
	(void)matrixpp::matrix<non_arithmetic_type>{};

	return 0;
}