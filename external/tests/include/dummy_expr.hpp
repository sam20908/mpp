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

#include <mpp/detail/expr_base.hpp>

#include <cstddef>

namespace mpp_test
{
	/**
	 * A dummy expression object for testing purposes
	 */
	template<typename Obj, std::size_t RowsExtent = Obj::rows_extent(), std::size_t ColumnsExtent = Obj::rows_extent()>
	class dummy_expr :
		public ::mpp::detail::expr_base<dummy_expr<Obj>, typename Obj::value_type, RowsExtent, ColumnsExtent>
	{
		const Obj& _obj;

	public:
		using value_type = typename Obj::value_type;

		dummy_expr(const Obj& obj) : _obj(obj) {} // @TODO: ISSUE #20

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return _obj.rows();
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return _obj.columns();
		}
		[[nodiscard]] auto at(std::size_t row_idx, std::size_t col_idx) const -> const value_type& // @TODO: ISSUE #20
		{
			return operator()(row_idx, col_idx);
		}

		[[nodiscard]] auto operator()(std::size_t row_idx, std::size_t col_idx) const
			-> const value_type& // @TODO: ISSUE #20
		{
			return _obj(row_idx, col_idx);
		}
	};
} // namespace mpp_test