// RUN: cd %binary_dir
// RUN: cmake -DTEST_NAME=customize_cpos -DTEST_SOURCE=%s -B build/customize_cpos
// RUN: cmake --build build/customize_cpos --target customize_cpos

#include <cassert>
#include <cstddef>
#include <matrixpp/algorithm.hpp>
#include <matrixpp/matrix.hpp>
#include <matrixpp/utility.hpp>
// No need to test operations because they aren't CPOs

namespace ns
{
	enum class dumb_type
	{
		lol
	};

	struct dumb_class
	{
	};

	struct dumb_class2
	{
		[[nodiscard]] constexpr auto operator==(const dumb_class2&) const -> bool = default;
	};

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::type_t, dumb_class) -> dumb_type
	{
		return dumb_type::lol;
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::cast_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::square_t, dumb_class) -> bool
	{
		return false;
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::block_t, dumb_class, int, int, int, int) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::determinant_t, dumb_class) -> int
	{
		return 2;
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::inverse_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(matrixpp::transpose_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}
} // namespace ns

int main()
{
	auto d = ns::dumb_class{};

	// Utility CPOs
	assert(matrixpp::type(d) == ns::dumb_type::lol);
	assert(matrixpp::cast(d) == ns::dumb_class2{});
	assert(!matrixpp::square(d));

	// Algorithm CPOs
	assert(matrixpp::block(d, 0, 0, 0, 0) == ns::dumb_class2{});
	assert(matrixpp::determinant(d) == 2);
	assert(matrixpp::inverse(d) == ns::dumb_class2{});
	assert(matrixpp::transpose(d) == ns::dumb_class2{});

	return 0;
}