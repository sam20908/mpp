// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=customize_default_extents -DTEST_SOURCE=%s -B build/customize_default_extents
// RUN: cmake --build build/customize_default_extents --target customize_default_extents

#include <matrixpp/utility/config.hpp>

namespace matrixpp::customize
{
	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_rows_extent_tag, customize_tag)
	{
		return 10;
	}

	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_columns_extent_tag, customize_tag)
	{
		return 10;
	}
} // namespace matrixpp::customize

#include <cassert>
#include <matrixpp/matrix.hpp>

int main()
{
	auto m = matrixpp::matrix<int>{};
	assert(m.rows_extent() == 10);
	assert(m.columns_extent() == 10);

	return 0;
}