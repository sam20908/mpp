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

#include <matrixpp/detail/expr_base.hpp>

namespace matrixpp_test::detail
{
	/**
	 * A dummy expression object for testing purposes
	 */
	template<typename Obj, std::size_t RowsExtent = Obj::rows_extent(), std::size_t ColumnsExtent = Obj::rows_extent()>
	class dummy_expr :
		public ::matrixpp::detail::expr_base<dummy_expr<Obj>, typename Obj::value_type, RowsExtent, ColumnsExtent>
	{
		const Obj& _obj;

	public:
		using value_type = typename Obj::value_type;

		dummy_expr(const Obj& obj) : _obj(obj) {} // @TODO: ISSUE #20

		[[nodiscard]] auto rows() const // @TODO: ISSUE #20
		{
			return _obj.rows();
		}

		[[nodiscard]] auto columns() const // @TODO: ISSUE #20
		{
			return _obj.columns();
		}
		[[nodiscard]] decltype(auto) at(std::size_t row_idx, std::size_t col_idx) const // @TODO: ISSUE #20
		{
			return operator()(row_idx, col_idx);
		}

		[[nodiscard]] decltype(auto) operator()(std::size_t row_idx, std::size_t col_idx) const // @TODO: ISSUE #20
		{
			return _obj(row_idx, col_idx);
		}
	};
} // namespace matrixpp_test::detail