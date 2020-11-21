#include <matrixpp/matrixpp.hpp>

// clang-format off
#include <array>
#include <ranges>
// clang-format on

int main()
{
	std::array<std::array<std::array<std::array<int, 1>, 1>, 1>, 1> rng_4d{ std::array{
		std::array{ std::array{ 1 } } } };

	(void)matrixpp::matrix<int>(rng_4d);

	return 0;
}