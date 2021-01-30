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

#include "../utility/config.hpp"
#include "constraints.hpp"
#include "tag_invoke.hpp"

#include <cstddef>
#include <initializer_list>

namespace matrixpp
{
	struct identity_matrix_tag
	{
	};

	inline constexpr auto identity_matrix = identity_matrix_tag{};

	template<detail::arithmetic Value,
		std::size_t RowsExtent    = detail::tag_invoke_cpo(matrix_rows_extent_tag{}, customize::customize_tag{}),
		std::size_t ColumnsExtent = detail::tag_invoke_cpo(matrix_columns_extent_tag{}, customize::customize_tag{})>
	class matrix;

	// Deduction guides

	template<typename Value>
	matrix(std::initializer_list<std::initializer_list<Value>>) -> matrix<Value>;

	// @TODO: Properly format this once ReferenceAlignment is implemented in clang-format
	// clang-format off
	template<detail::range_2d_arithmetic Range2D>
	matrix(Range2D&&) -> matrix<detail::range_2d_t<Range2D>>;
	// clang-format on

	template<typename Expr, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	matrix(const detail::expr_base<Expr, Value, RowsExtent, ColumnsExtent>&)
		-> matrix<Value, RowsExtent, ColumnsExtent>;
} // namespace matrixpp