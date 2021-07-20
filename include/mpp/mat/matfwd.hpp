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

#pragma once

#include <mpp/detail/expr/expr_base.hpp>
#include <mpp/detail/utility/public.hpp>
#include <mpp/detail/utility/utility.hpp>
#include <mpp/utility/cfg.hpp>

#include <cstddef>
#include <initializer_list>

namespace mpp
{
	/**
	 * Forward declarations
	 */

	template<detail::arithmetic Val,
		std::size_t Rows = cfg<override>::rows_extent,
		std::size_t Cols = cfg<override>::cols_extent,
		typename Alloc   = typename cfg<override>::alloc<Val>>
	class mat;

	/**
	 * Aliases
	 */

	// mat2v aliases

	template<typename Val, typename Alloc = std::allocator<Val>>
	using mat2v = mpp::mat<Val, 2, 2, Alloc>;

	template<typename Alloc = std::allocator<int>>
	using mat2i = mat2v<int, Alloc>;

	template<typename Alloc = std::allocator<float>>
	using mat2f = mat2v<float, Alloc>;

	template<typename Alloc = std::allocator<double>>
	using mat2d = mat2v<double, Alloc>;

	// mat3v aliases

	template<typename Val, typename Alloc = typename cfg<override>::alloc<Val>>
	using mat3v = mpp::mat<Val, 3, 3, Alloc>;

	template<typename Alloc = std::allocator<int>>
	using mat3i = mat3v<int, Alloc>;

	template<typename Alloc = std::allocator<float>>
	using mat3f = mat3v<float, Alloc>;

	template<typename Alloc = std::allocator<double>>
	using mat3d = mat3v<double, Alloc>;
	// vector aliases

	template<typename Val, std::size_t Cols, typename Alloc = typename cfg<override>::alloc<Val>>
	using row_vec = mpp::mat<Val, 1, Cols, Alloc>;

	template<typename Val, std::size_t Rows, typename Alloc = typename cfg<override>::alloc<Val>>
	using col_vec = mpp::mat<Val, Rows, 1, Alloc>;

	/**
	 * Deduction guides
	 */

	template<typename Val>
	mat(std::initializer_list<std::initializer_list<Val>>) -> mat<Val>;

	template<typename Rng>
	mat(Rng&&) -> mat<detail::range_2d_value_t<Rng>>;

	template<typename Expr, typename Val, std::size_t Rows, std::size_t Cols>
	mat(const detail::expr_base<Expr, Val, Rows, Cols>&) -> mat<Val, Rows, Cols>;

	template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	mat(const mat<Val, Rows, Cols, Alloc>&, const Alloc&) -> mat<Val, Rows, Cols, Alloc>;

	template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
	mat(mat<Val, Rows, Cols, Alloc>&&, const Alloc&) -> mat<Val, Rows, Cols, Alloc>;
} // namespace mpp
