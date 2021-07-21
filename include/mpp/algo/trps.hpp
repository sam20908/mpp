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

#include <mpp/detail/util/cpo_base.hpp>
#include <mpp/detail/util/util.hpp>
#include <mpp/mat/matfwd.hpp>

#include <cstddef>
#include <type_traits>

namespace mpp
{
	namespace detail
	{
		template<typename To>
		[[nodiscard]] auto trps_impl(const auto& obj) -> To
		{
			const auto rows = obj.rows();
			const auto cols = obj.cols();
			const auto data = obj.data();

			using trps_buf_t = typename To::buffer_type;

			auto trps_buf = trps_buf_t{};
			resize_buf_if_vec(trps_buf, cols, rows, typename To::value_type{});

			for (auto col = std::size_t{}; col < cols; ++col)
			{
				for (auto row = std::size_t{}; row < rows; ++row)
				{
					auto idx      = detail::idx_1d(cols, row, col);
					auto trps_idx = detail::idx_1d(rows, col, row);

					trps_buf[trps_idx] = data[idx];
				}
			}

			return To{ cols, rows, std::move(trps_buf) };
		}
	} // namespace detail

	struct trps_t : public detail::cpo_base<trps_t>
	{
		template<typename Val,
			std::size_t Rows,
			std::size_t Cols,
			typename Alloc,
			typename To = mat<Val, Cols, Rows, Alloc>>
		requires(detail::is_matrix<To>::value) [[nodiscard]] friend inline auto tag_invoke(trps_t,
			const mat<Val, Rows, Cols, Alloc>& obj,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::trps_impl<To>(obj);
		}
	};

	inline constexpr auto trps = trps_t{};
} // namespace mpp
