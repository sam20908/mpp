#include <matrixpp/matrixpp.hpp>

// clang-format off
#include <array>
#include <ranges>
// clang-format on

int main()
{
	std::array<std::array<std::array<int, 1>, 1>, 1> rng_3d{ std::array{ std::array{ 1 } } };

	(void)matrixpp::matrix<int>(rng_3d);

	return 0;
}