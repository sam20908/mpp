#include <matrixpp/matrixpp.hpp>

struct non_arithmetic_type
{
};

int main()
{
	matrixpp::matrix<non_arithmetic_type>;

	return 0;
}