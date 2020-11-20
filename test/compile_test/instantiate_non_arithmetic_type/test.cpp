#include <matrixpp/matrixpp.hpp>

struct non_arithmetic_type
{
};

int main()
{
    matrixpp::matrix<non_arithmetic_type>;

    // @TODO: Add test case for static, dynamic rows, and dynamic columns matrices

	return 0;
}