export const kinds = `#include <matrixpp/matrix.h>

namespace mpp = matrixpp;

int main()
{
    auto m_fully_static = mpp::matrix<int, 3, 3>{};
    auto m_fully_dynamic = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>{};
    // ^ or just mpp::matrix<int>{} since the size parameters are defaulted to dynamic extent
    auto m_dynamic_rows = mpp::matrix<int, std::dynamic_Extent, 3>{};
    auto m_dynamic_columns = mpp::matrix<int, 3, std::dynamic_extent>{};
    // ^ or just mpp::matrix<int, 3>{} since the column parameter is defaulted to dynamic extent

    return 0;
}`;

export const operations = `#include <matrixpp/matrix.h>
#include <matrixpp/operations.hpp> // +, -, *, and /
#include <matrixpp/algorithms.hpp> // determinant, inverse, transpose, block, and etc...
#include <utility> // std::type_identity

namespace mpp = matrixpp;

int main()
{
    auto m = mpp::matrix{{
        { 1, 2, 3 },
        { 1, 2, 3 },
        { 1, 2, 3 }
    }}; // Uses 2D initializer list initializer. Defaults to dynamic matrix

    // Remember that math operations are expression templates, so the results are not evaluated immediately
    auto expr = m + m - m * 2 + m / 3;

    auto result = expr(0, 0); // This only evaluates the result at (1, 1)
    auto result_m = mpp::matrix{ expr }; // Evaluates the whole expression and constructs a matrix

    auto m_det = mpp::determinant(m); // You can specify larger types for safety to avoid type overflow from calculation

    auto m_inv = mpp::inverse(std::type_identity<float>{}, m); // Use "float" for result matrix type
    auto m_inv_2 = mpp::inverse(std::type_identity<int>{}, m); // Compile error: user provided type must be a floating point!
    auto m_inv_int = mpp::inverse(m); // Defaults to "double" for result matrix type

    auto m_transposed = mpp::transpose(m);
    auto m_block = mpp::block(m, 0, 0, 1, 1); // Grabs top corner 2 x 2 (the indexes are inclusive, so 1 x 1
                                              // blocks are possible :) )

    return 0;
}`;

export const customize_default_extent = `#include <matrixpp/utility/config.hpp>

// It's very important to do this before including matrix.hpp, otherwise the customization
// is not going to be detected
namespace matrixpp::customize
{
    // Customization for default extent for matrix class has to take place here because
    // the library looks for customizations via ADL, and customize_tag is declared in this
    // scope
    [[nodiscard]] constexpr std::size_t tag_invoke(matrix_rows_extent_tag, customize_tag)
    {
        return 10;
    }

    [[nodiscard]] constexpr std::size_t tag_invoke(matrix_columns_extent_tag, customize_tag)
    {
        return 10;
    }
} // namespace matrixpp::customize

#include <matrixpp/matrix.hpp>

namespace mpp = matrixpp;

int main()
{
    auto m = mpp::matrix<int>{}; // The library uses the new default extent by default
    auto r = m.rows_extent(); // 10
    auto c = m.columns_extent(); // 10

    return 0;
}
`;
