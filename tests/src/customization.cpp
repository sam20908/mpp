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

#include <mpp/util/cfg.hpp>

#include "../include/custom_allocator.hpp"

#include <array>
#include <vector>

namespace mpp
{
	template<>
	struct cfg<override>
	{
		template<typename Val>
		using alloc = custom_allocator<Val>;

		static constexpr std::size_t rows_extent = 10;
		static constexpr std::size_t cols_extent = 10;

		static constexpr bool use_unsafe = true;

		// Only for testing purposes, don't actually do this!

		template<typename Val, std::size_t, std::size_t, typename>
		using fixed_buf = std::vector<Val>;

		template<typename Val, std::size_t, std::size_t, typename>
		using dyn_buf = std::array<Val, 100>;

		template<typename Val, std::size_t, std::size_t, typename>
		using dyn_rows_buf = std::array<Val, 100>;
		template<typename Val, std::size_t, std::size_t, typename>
		using dyn_cols_buf = std::array<Val, 100>;
	};
} // namespace mpp

#include <boost/ut.hpp>

#include <mpp/algo.hpp>
#include <mpp/mat.hpp>
#include <mpp/util.hpp>

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

	[[nodiscard]] constexpr auto tag_invoke(mpp::sq_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::block_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::det_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::inverse_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::trps_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::sg_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::size_compare_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::cmp_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::lu_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::fwd_sub_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(mpp::back_sub_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}
} // namespace ns

template<typename CPO>
using invoke_result_t = mpp::detail::tag_invoke_result_t<CPO, ns::dumb_class>;

int main()
{
	using namespace boost::ut::literals;
	using namespace boost::ut::bdd;
	using namespace boost::ut;

	when("I check the new extents through the default matrix type") = []() {
		expect(mpp::mat<int>::rows_extent() == 10_ul);
		expect(mpp::mat<int>::cols_extent() == 10_ul);
	};

	when("I check against the CPOs' return types") = []() {
		expect(type<invoke_result_t<mpp::type_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::sg_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::sq_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::block_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::det_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::inverse_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::trps_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::size_compare_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::cmp_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::lu_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::fwd_sub_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<mpp::back_sub_t>> == type<ns::dumb_class2>);
	};

	when("I check the customized buffer types") = []() {
		expect(type<typename mpp::mat<int, 2, 3>::buffer_type> == type<std::vector<int>>);
		expect(type<typename mpp::mat<int, mpp::dyn, mpp::dyn>::buffer_type> == type<std::array<int, 100>>);
		expect(type<typename mpp::mat<int, mpp::dyn, 3>::buffer_type> == type<std::array<int, 100>>);
		expect(type<typename mpp::mat<int, 2, mpp::dyn>::buffer_type> == type<std::array<int, 100>>);
	};

	// Putting semiregular test here because this test does all the constants/type checking stuff

	scenario("CPOs should meet std::semiregular requirements") = []() {
		expect(constant<std::semiregular<mpp::type_t>>);
		expect(constant<std::semiregular<mpp::sg_t>>);
		expect(constant<std::semiregular<mpp::sq_t>>);
		expect(constant<std::semiregular<mpp::block_t>>);
		expect(constant<std::semiregular<mpp::det_t>>);
		expect(constant<std::semiregular<mpp::inverse_t>>);
		expect(constant<std::semiregular<mpp::trps_t>>);
		expect(constant<std::semiregular<mpp::size_compare_t>>);
		expect(constant<std::semiregular<mpp::cmp_t>>);
		expect(constant<std::semiregular<mpp::lu_t>>);
		expect(constant<std::semiregular<mpp::fwd_sub_t>>);
		expect(constant<std::semiregular<mpp::back_sub_t>>);
	};

	when("I check the customized allocator type") = []() {
		expect(type<typename mpp::mat<int, 2, 3>::allocator_type> == type<custom_allocator<int>>);
		expect(type<typename mpp::mat<int, mpp::dyn, mpp::dyn>::allocator_type> == type<custom_allocator<int>>);
		expect(type<typename mpp::mat<int, mpp::dyn, 3>::allocator_type> == type<custom_allocator<int>>);
		expect(type<typename mpp::mat<int, 2, mpp::dyn>::allocator_type> == type<custom_allocator<int>>);
	};

	return 0;
}
