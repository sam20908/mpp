#include <matrixpp/matrixpp.hpp>

// clang-format off
#include <array>
#include <ranges>
// clang-format on

int main()
{
	std::array rng_1d{ 1, 2, 3, 4, 5, 6 };

	(void)matrixpp::matrix<int>(rng_1d);

	return 0;
}