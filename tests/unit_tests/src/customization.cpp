/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

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

#include <gtest/gtest.h>

#include <matrixpp/algorithm.hpp>
#include <matrixpp/matrix.hpp>
#include <matrixpp/utility.hpp>

namespace
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

	TEST(Customization, DefaultExtents)
	{
		auto m = matrixpp::matrix<int>{};
		EXPECT_EQ(m.rows_extent(), 10);
		EXPECT_EQ(m.columns_extent(), 10);
	}

	TEST(Customization, Type)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::type(d), dumb_type::lol);
	}

	TEST(Customization, Cast)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::cast(d), dumb_class2{});
	}

	TEST(Customization, Square)
	{
		auto d = dumb_class{};
		EXPECT_FALSE(matrixpp::square(d));
	}

	TEST(Customization, Block)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::block(d, 0, 0, 0, 0), dumb_class2{});
	}

	TEST(Customization, Determinant)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::determinant(d), 2);
	}

	TEST(Customization, Inverse)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::inverse(d), dumb_class2{});
	}

	TEST(Customization, Transpose)
	{
		auto d = dumb_class{};
		EXPECT_EQ(matrixpp::transpose(d), dumb_class2{});
	}
} // namespace