#include <matrixpp/matrixpp.hpp>
#include <span>

struct non_arithmetic_type
{
};

int main()
{
	matrixpp::matrix<non_arithmetic_type>;                         // Fully dynamic
	matrixpp::matrix<non_arithmetic_type, 0, 0>;                   // Fully static
	matrixpp::matrix<non_arithmetic_type, std::dynamic_extent, 1>; // Dynaic rows
	matrixpp::matrix<non_arithmetic_type, 1, std::dynamic_extent>; // Dynamic columns

	return 0;
}