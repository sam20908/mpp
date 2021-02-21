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

// RUN: %build_and_run

#include <mpp/matrix.hpp>

#include "../../include/utility_traits.hpp"

#include <concepts>
#include <iterator>
#include <memory>
#include <span>

/**
 * @NOTE: it "technically" does not meet all required expressions stated because of how different matrices have
 * different sets of constructors and how it has to be treated in matrix form
 *
 * Also, the Standardese explicitly states that std::array is excluded from SequenceContainer, therefore fully static
 * matrices are also excluded from SequenceContainer
 *
 * The following expressions are omitted:
 * - T(n, t)
 * - T(begin, end)
 * - t.emplace(pos, args)
 * - t.insert(pos, val) and all other overloads
 * - t.erase(pos, val) and all other overloads
 * - t.assign(begin, end)
 * - t.assign(n, t)
 */

template<typename T>
concept container = requires(T t, T u, const T ct, T&& rt)
{
	typename mpp_test::container_value_type<T>;
	typename mpp_test::container_reference<T>;
	typename mpp_test::container_const_reference<T>;
	typename mpp_test::container_iterator<T>;
	typename mpp_test::container_const_iterator<T>;
	typename mpp_test::container_difference_type<T>;
	typename mpp_test::container_size_type<T>;
	requires std::default_initializable<T>;
	requires std::destructible<T>;
	{ T{ t } };
	{ T{ rt } };
	{ t = u };
	{ t = rt };
	// @TODO: Enable equality comparable when #130 is complete
	// requires std::equality_comparable<T>;
	{
		t.begin()
	}
	->std::same_as<typename mpp_test::container_iterator<T>>;
	{
		t.end()
	}
	->std::same_as<typename mpp_test::container_iterator<T>>;
	{
		t.cbegin()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{
		t.cend()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{
		ct.begin()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{
		ct.end()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{
		ct.cbegin()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{
		ct.cend()
	}
	->std::same_as<typename mpp_test::container_const_iterator<T>>;
	{ t.swap(u) };
	{ swap(t, u) };
	{ t.size() };
	{ t.max_size() };
	{
		t.empty()
	}
	->std::convertible_to<bool>;
};

template<typename T>
concept sequence_container = requires(T t,
	const T ct,
	typename mpp_test::container_iterator<T> begin,
	typename mpp_test::container_iterator<T> end,
	std::initializer_list<std::initializer_list<typename T::value_type>> init_2d)
{
	// @NOTE: Inconvenient to test reference convertibility of base class to derived class
	{ t = init_2d };
	{
		t.front()
	}
	->std::same_as<typename mpp_test::container_reference<T>>;
	{
		ct.front()
	}
	->std::same_as<typename mpp_test::container_const_reference<T>>;
	{
		t.back()
	}
	->std::same_as<typename mpp_test::container_reference<T>>;
	{
		ct.back()
	}
	->std::same_as<typename mpp_test::container_const_reference<T>>;
	{ t.clear() };
	{ t.assign(init_2d) };
};

template<typename T>
concept reversible_container = requires(T t)
{
	typename mpp_test::container_reverse_iterator<T>;
	typename mpp_test::container_const_reverse_iterator<T>;
	{
		t.rbegin()
	}
	->std::same_as<typename mpp_test::container_reverse_iterator<T>>;
	{
		t.rend()
	}
	->std::same_as<typename mpp_test::container_reverse_iterator<T>>;
	{
		t.crbegin()
	}
	->std::same_as<typename mpp_test::container_const_reverse_iterator<T>>;
	{
		t.crend()
	}
	->std::same_as<typename mpp_test::container_const_reverse_iterator<T>>;
};

int main()
{
	using fully_static    = mpp::matrix<int, 1, 1>;
	using fully_dynamic   = mpp::matrix<int, std::dynamic_extent, std::dynamic_extent>;
	using dynamic_rows    = mpp::matrix<int, std::dynamic_extent, 1>;
	using dynamic_columns = mpp::matrix<int, 1, std::dynamic_extent>;

	static_assert(container<fully_static>, "Fully static matrices must meet Container!");
	static_assert(container<fully_dynamic>, "Fully dynamic matrices must meet Container!");
	static_assert(container<dynamic_rows>, "Dynamic row matrices must meet Container!");
	static_assert(container<dynamic_columns>, "Dynamic column matrices must meet Container!");

	static_assert(sequence_container<fully_dynamic>, "Fully dynamic matrices must meet SequenceContainer!");
	static_assert(sequence_container<dynamic_rows>, "Dynamic row matrices must meet SequenceContainer!");
	static_assert(sequence_container<dynamic_columns>, "Dynamic column matrices must meet SequenceContainer!");

	static_assert(reversible_container<fully_static>, "Fully static matrices must meet ReversibleContainer!");
	static_assert(reversible_container<fully_dynamic>, "Fully dynamic matrices must meet ReversibleContainer!");
	static_assert(reversible_container<dynamic_rows>, "Dynamic row matrices must meet ReversibleContainer!");
	static_assert(reversible_container<dynamic_columns>, "Dynamic column matrices must meet ReversibleContainer!");

	return 0;
}