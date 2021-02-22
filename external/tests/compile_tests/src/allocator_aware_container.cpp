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
#include <memory>
#include <span>

/**
 * @NOTE: it "technically" does not meet all required expressions stated because of how different matrices have
 * different sets of constructors
 *
 * This entire test is omitted for static matrices because it does not use an allocator
 */

template<typename T, typename Allocator, typename... SizeParams>
concept initializable_with_allocator =
	std::constructible_from<T, Allocator>&& std::constructible_from<T, SizeParams..., Allocator>&&
		std::constructible_from<T, T, Allocator>&& std::constructible_from<T, T&&, Allocator>;

template<typename T, typename... SizeParams>
concept allocator_aware = std::default_initializable<T>&& std::move_constructible<T>&&
	initializable_with_allocator<T, mpp_test::container_allocator_type<T>, SizeParams...>&& requires(T t,
		T u,
		const T& v,
		T&& rv)
{
	typename mpp_test::container_allocator_type<T>;
	// @NOTE: Cannot put initializable_with_allocator here because GCC ICEs here
	{
		t.get_allocator()
	}
	->std::same_as<mpp_test::container_allocator_type<T>>;
	{ t = v };
	{ t = rv };
	{ t.swap(u) };
};

int main()
{
	using fully_dynamic_matrix   = mpp::matrix<int>;
	using dynamic_rows_matrix    = mpp::matrix<int, std::dynamic_extent, 1>;
	using dynamic_columns_matrix = mpp::matrix<int, 1, std::dynamic_extent>;

	static_assert(allocator_aware<fully_dynamic_matrix, std::size_t, std::size_t>,
		"Fully dynamic matrices must meet AllocatorAwareContainer!");

	static_assert(allocator_aware<dynamic_rows_matrix, std::size_t>,
		"Dynamic rows matrices must meet AllocatorAwareContainer!");

	static_assert(allocator_aware<dynamic_columns_matrix, std::size_t>,
		"Dynamic columns matrices must meet AllocatorAwareContainer!");

	return 0;
}