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

#include <mpp/utility/config_extents.hpp>

namespace mpp::customize_extents
{
	[[nodiscard]] constexpr std::size_t tag_invoke(rows_extent_tag, customize_tag)
	{
		return 10;
	}

	[[nodiscard]] constexpr std::size_t tag_invoke(columns_extent_tag, customize_tag)
	{
		return 10;
	}
} // namespace mpp::customize_extents

#include <mpp/algorithm.hpp>
#include <mpp/matrix.hpp>
#include <mpp/utility.hpp>

#include "../../thirdparty/ut.hpp"

namespace ns
{
	struct dumb_class
	{
	};

	struct dumb_class2
	{
	};

	[[nodiscard]] constexpr auto tag_invoke(mpp::type_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::square_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::block_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::determinant_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::inverse_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::transpose_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::singular_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::size_compare_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::elements_compare_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}
} // namespace ns

template<typename CPO>
using invoke_result_t = mpp::detail::tag_invoke_result_t<CPO, ns::dumb_class>;

int main()
{
	using namespace boost::ut::literals;
	using namespace boost::ut::operators::terse;
	using namespace boost::ut::bdd;
	using namespace boost::ut;

	"customization"_test = []() {
		when("I check the new extents through the default matrix type") = []() {
			expect(constant<mpp::matrix<int>::rows_extent() == 10_ul>);
			expect(constant<mpp::matrix<int>::columns_extent() == 10_ul>);
		};

		when("I check against the CPOs' return types") = []() {
			expect(type<invoke_result_t<mpp::type_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::singular_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::square_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::block_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::determinant_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::inverse_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::transpose_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::size_compare_t>> == type<ns::dumb_class2>);
			expect(type<invoke_result_t<mpp::elements_compare_t>> == type<ns::dumb_class2>);
		};
	};

	return 0;
}