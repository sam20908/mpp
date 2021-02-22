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

#include <mpp/utility/config.hpp>

namespace mpp::customize
{
	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_rows_extent_tag, customize_tag)
	{
		return 10;
	}

	[[nodiscard]] constexpr std::size_t tag_invoke(matrix_columns_extent_tag, customize_tag)
	{
		return 10;
	}
} // namespace mpp::customize

#include <gtest/gtest.h>

#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>
#include <mpp/utility.hpp>

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

	[[nodiscard]] constexpr auto tag_invoke(mpp::type_t, dumb_class) -> dumb_type
	{
		return dumb_type::lol;
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::cast_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::square_t, dumb_class) -> bool
	{
		return false;
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::block_t, dumb_class, int, int, int, int) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::determinant_t, dumb_class) -> int
	{
		return 2;
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::inverse_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::transpose_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::singular_t, dumb_class) -> bool
	{
		return true;
	}

	TEST(Customization, DefaultExtents)
	{
		const auto matrix = mpp::matrix<int>{};
		EXPECT_EQ(matrix.rows_extent(), 10);
		EXPECT_EQ(matrix.columns_extent(), 10);
	}

	TEST(Customization, Type)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::type(dummy), dumb_type::lol);
	}

	TEST(Customization, Cast)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::cast(dummy), dumb_class2{});
	}

	TEST(Customization, Square)
	{
		const auto dummy = dumb_class{};
		EXPECT_FALSE(mpp::square(dummy));
	}

	TEST(Customization, Block)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::block(dummy, 0, 0, 0, 0), dumb_class2{});
	}

	TEST(Customization, Determinant)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::determinant(dummy), 2);
	}

	TEST(Customization, Inverse)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::inverse(dummy), dumb_class2{});
	}

	TEST(Customization, Transpose)
	{
		const auto dummy = dumb_class{};
		EXPECT_EQ(mpp::transpose(dummy), dumb_class2{});
	}

	TEST(Customization, Singular)
	{
		const auto dummy = dumb_class{};
		EXPECT_TRUE(mpp::singular(dummy));
	}

	// @TODO: Add customization test for upcoming equal CPOs
} // namespace