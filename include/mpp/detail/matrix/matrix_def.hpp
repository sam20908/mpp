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
#include <mpp/detail/types/constraints.hpp>
#include <mpp/detail/utility/public.hpp> // @TODO: Export this header to the user for modules
#include <mpp/utility/configuration.hpp>

#include <cstddef>
#include <initializer_list>

namespace mpp
{
	template<detail::arithmetic Value,
		std::size_t RowsExtent    = configuration<override>::rows_extent,
		std::size_t ColumnsExtent = configuration<override>::columns_extent,
		typename Allocator        = typename configuration<override>::allocator<Value>>
	class matrix;

	/**
	 * Aliases
	 */

	// matrix2v aliases

	template<typename Value, typename Allocator = std::allocator<Value>>
	using matrix2v = mpp::matrix<Value, 2, 2, Allocator>;

	template<typename Allocator = std::allocator<int>>
	using matrix2i = matrix2v<int, Allocator>;

	template<typename Allocator = std::allocator<float>>
	using matrix2f = matrix2v<float, Allocator>;

	template<typename Allocator = std::allocator<double>>
	using matrix2d = matrix2v<double, Allocator>;

	// matrix3v aliases

	template<typename Value, typename Allocator = std::allocator<Value>>
	using matrix3v = mpp::matrix<Value, 3, 3, Allocator>;

	template<typename Allocator = std::allocator<int>>
	using matrix3i = matrix3v<int, Allocator>;

	template<typename Allocator = std::allocator<float>>
	using matrix3f = matrix3v<float, Allocator>;

	template<typename Allocator = std::allocator<double>>
	using matrix3d = matrix3v<double, Allocator>;
	// vector aliases

	template<typename Value, std::size_t ColumnsExtent, typename Allocator = std::allocator<Value>>
	using row_vector = mpp::matrix<Value, 1, ColumnsExtent, Allocator>;

	template<typename Value, std::size_t RowsExtent, typename Allocator = std::allocator<Value>>
	using column_vector = mpp::matrix<Value, RowsExtent, 1, Allocator>;

	/**
	 * Deduction guides
	 */

	template<typename Value>
	matrix(std::initializer_list<std::initializer_list<Value>>) -> matrix<Value>;

	template<typename Range2D>
	matrix(Range2D&&) -> matrix<detail::range_2d_value_t<Range2D>>;

	template<typename Expr, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	matrix(const detail::expr_base<Expr, Value, RowsExtent, ColumnsExtent>&)
		-> matrix<Value, RowsExtent, ColumnsExtent>;

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
	matrix(const matrix<Value, RowsExtent, ColumnsExtent, Allocator>&, const Allocator&)
		-> matrix<Value, RowsExtent, ColumnsExtent, Allocator>;

	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
	matrix(matrix<Value, RowsExtent, ColumnsExtent, Allocator>&&, const Allocator&)
		-> matrix<Value, RowsExtent, ColumnsExtent, Allocator>;
} // namespace mpp
