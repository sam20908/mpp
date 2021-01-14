// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=customization -DTEST_SOURCE=%s -B build/customization
// RUN: cmake --build build/customization --target customization

#include <matrixpp/utility/config.hpp>

namespace matrixpp::customize
{
	template<>
	[[nodiscard]] constexpr std::size_t default_rows_extent<customize_tag>()
	{
		return 10;
	}

	template<>
	[[nodiscard]] constexpr std::size_t default_columns_extent<customize_tag>()
	{
		return 10;
	}
} // namespace matrixpp::customize

#include <cassert>
#include <matrixpp/matrix.hpp>

int main()
{
	static_assert(matrixpp::default_rows_extent() == 10, "Customized rows extent not detected!");
	static_assert(matrixpp::default_columns_extent() == 10, "Customized columns extent not detected!");

	auto m = matrixpp::matrix<int>{};
	assert(m.rows_extent() == 10);
	assert(m.columns_extent() == 10);

	return 0;
}