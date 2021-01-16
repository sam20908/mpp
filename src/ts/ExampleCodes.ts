export const init = `#include <matrixpp/matrix.h>

namespace mpp = matrixpp; // More convenient namespace alias

int main()
{
    auto m = mpp::matrix<int>{};

    return 0;
}`;
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
#include <matrixpp/operations.hpp> // Required for +, -, *, and /
#include <matrixpp/algorithms.hpp> // Required for determinant, inverse, transpose, block, and etc...

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
    auto m_inv = mpp::inverse<float>(m); // Optional template parameter to allow custom precision types,
                                         // since inverse usually outputs floatings, it's a good candidate
    auto m_transposed = mpp::transpose(m);
    auto m_block = mpp::block(m, 0, 0, 1, 1); // Grabs top corner 2 x 2 (the indexes are inclusive, so 1 x 1
                                              // blocks are possible :) )

    return 0;
}`;
