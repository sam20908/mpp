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

#include <mpp/detail/constraints.hpp>
#include <mpp/detail/expr_base.hpp>
#include <mpp/detail/tag_invoke.hpp>

// @TODO: Make sure to export these modules for C++20
#include <mpp/detail/public_tags.hpp>
#include <mpp/utility/config_extents.hpp>

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace mpp
{
	template<detail::arithmetic Value,
		std::size_t RowsExtent =
			detail::tag_invoke_cpo_constexpr(customize_extents::rows_extent, customize_extents::customize),
		std::size_t ColumnsExtent =
			detail::tag_invoke_cpo_constexpr(customize_extents::columns_extent, customize_extents::customize),
		typename Allocator = std::allocator<Value>>
	class matrix;

	/**
	 * Deduction guides
	 */

	template<typename Value>
	matrix(std::initializer_list<std::initializer_list<Value>>) -> matrix<Value>;

	// @TODO: Properly format this once ReferenceAlignment is implemented in clang-format
	// clang-format off
	template<typename Range2D>
	matrix(Range2D&&) -> matrix<detail::range_2d_value_t<Range2D>>;
	// clang-format on

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