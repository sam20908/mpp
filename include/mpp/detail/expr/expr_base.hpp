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
	template<typename Expr, typename Val, std::size_t Rows, std::size_t Cols>
	class [[nodiscard]] expr_base
	{
	protected:
		[[nodiscard]] constexpr auto expr() const noexcept -> const Expr&
		{
			return static_cast<const Expr&>(*this);
		}

		[[nodiscard]] constexpr auto expr_mut() noexcept -> Expr&
		{
			return static_cast<Expr&>(*this);
		}

	public:
		using value_type = Val;

		[[nodiscard]] auto rows() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return expr().rows();
		}

		[[nodiscard]] auto cols() const noexcept -> std::size_t // @TODO: ISSUE #20
		{
			return expr().cols();
		}

		[[nodiscard]] constexpr static auto rows_extent() noexcept -> std::size_t
		{
			return Rows;
		}

		[[nodiscard]] constexpr static auto cols_extent() noexcept -> std::size_t
		{
			return Cols;
		}

		[[nodiscard]] auto operator()(std::size_t row, std::size_t col) const noexcept -> Val // @TODO: ISSUE #20
		{
			return expr()(row, col);
		}
	};
} // namespace mpp::detail