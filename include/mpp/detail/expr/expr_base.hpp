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

#include <cstddef>
#include <stdexcept>

namespace mpp::detail
{
	/**
	 * Base class for expression objects
	 */
	template<typename Expr, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	class [[nodiscard]] expr_base
	{
		[[nodiscard]] constexpr auto expr_obj() const -> const Expr&
		{
			// Grabs reference to the subclass
			return static_cast<const Expr&>(*this);
		}

	public:
		using value_type = Value;

		[[nodiscard]] auto rows() const -> std::size_t // @TODO: ISSUE #20
		{
			return expr_obj().rows();
		}

		[[nodiscard]] auto columns() const -> std::size_t // @TODO: ISSUE #20
		{
			return expr_obj().columns();
		}

		[[nodiscard]] constexpr static auto rows_extent() -> std::size_t
		{
			return RowsExtent;
		}

		[[nodiscard]] constexpr static auto columns_extent() -> std::size_t
		{
			return ColumnsExtent;
		}

		[[nodiscard]] auto at(std::size_t row_index, std::size_t col_index) const -> value_type // @TODO: ISSUE #20
		{
			if (row_index >= rows() || col_index >= columns())
			{
				throw std::out_of_range("Access out of range!");
			}

			return expr_obj().at(row_index, col_index);
		}

		[[nodiscard]] auto operator()(std::size_t row_index, std::size_t col_index) const -> value_type // @TODO: ISSUE #20
		{
			return expr_obj()(row_index, col_index);
		}
	};
} // namespace mpp::detail