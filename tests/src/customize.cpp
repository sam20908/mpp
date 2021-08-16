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

#include <boost/ut.hpp>

#include <mpp/algo.hpp>
#include <mpp/mat.hpp>
#include <mpp/util.hpp>

using namespace boost::ut::literals;
using namespace boost::ut::bdd;
using namespace boost::ut;
using namespace mpp;

template<typename T>
struct wacky_array
{
	T data[4096];

	using pointer                = T*;
	using const_pointer          = const T*;
	using iterator               = T*;
	using const_iterator         = T*;
	using reverse_iterator       = T*;
	using const_reverse_iterator = T*;

	using difference_type = std::size_t;
};

template<typename T>
inline constexpr auto is_fixed_buf<wacky_array<T>> = true;

namespace ns
{
	struct dumb_class
	{
	};

	struct dumb_class2
	{
	};

	[[nodiscard]] constexpr auto tag_invoke(block_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(det_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(inv_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(trps_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(cmp_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(lu_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(fwd_sub_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}

	[[nodiscard]] constexpr auto tag_invoke(back_sub_t, dumb_class) -> dumb_class2
	{
		return dumb_class2{};
	}
} // namespace ns

template<typename CPO>
using invoke_result_t = mpp::detail::tag_invoke_result_t<CPO, ns::dumb_class>;

int main()
{
	when("I check against the CPOs' return types") = []() {
		expect(type<invoke_result_t<block_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<det_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<inv_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<trps_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<cmp_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<lu_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<fwd_sub_t>> == type<ns::dumb_class2>);
		expect(type<invoke_result_t<back_sub_t>> == type<ns::dumb_class2>);
	};

	when("I check the properties of the matrix with customized buffer") = []() {
		using wacky_mat = mat<int, wacky_array<int>>;

		expect(type<typename wacky_mat::value_type> == type<int>);
		expect(type<typename wacky_mat::buffer_type> == type<wacky_array<int>>);

		expect(type<typename wacky_mat::reference> == type<int&>);
		expect(type<typename wacky_mat::const_reference> == type<const int&>);
		expect(type<typename wacky_mat::pointer> == type<int*>);
		expect(type<typename wacky_mat::const_pointer> == type<const int*>);

		expect(std::contiguous_iterator<typename wacky_mat::iterator>);
		expect(std::contiguous_iterator<typename wacky_mat::const_iterator>);
		expect(std::bidirectional_iterator<typename wacky_mat::reverse_iterator>);
		expect(std::bidirectional_iterator<typename wacky_mat::const_reverse_iterator>);

		expect(type<typename wacky_mat::difference_type> == type<std::size_t>);
	};

	scenario("CPOs should meet std::semiregular requirements") = []() {
		expect(constant<std::semiregular<block_t>>);
		expect(constant<std::semiregular<det_t>>);
		expect(constant<std::semiregular<inv_t>>);
		expect(constant<std::semiregular<trps_t>>);
		expect(constant<std::semiregular<cmp_t>>);
		expect(constant<std::semiregular<lu_t>>);
		expect(constant<std::semiregular<fwd_sub_t>>);
		expect(constant<std::semiregular<back_sub_t>>);
	};

	return 0;
}
