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

#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/square.hpp>
#include <mpp/matrix.hpp>

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename To2, typename Mat>
		auto lu_impl(const Mat& obj) -> std::pair<To, To2> // @TODO: ISSUE #20
		{
			assert(square(obj));

			using lu_buffer_t = typename mat_rebind_to_t<Mat, default_floating_type>::buffer_type;

			const auto rows    = obj.rows();
			const auto columns = obj.columns();

			auto l_buffer = lu_buffer_t{};
			auto u_buffer = lu_buffer_t{};

			// @TODO: Should do a direct buffer copy initialization instead
			allocate_buffer_if_vector(u_buffer, rows, columns, default_floating_type{});
			std::ranges::copy(obj, u_buffer.begin());

			make_identity_buffer(l_buffer, rows, columns, default_floating_type{}, default_floating_type{ 1 });

			lu_generic<default_floating_type, true, false>(rows, columns, l_buffer, u_buffer);

			return { To{ rows, columns, std::move(l_buffer) }, To2{ rows, columns, std::move(u_buffer) } };
		}
	} // namespace detail

	struct lu_decomposition_t : public detail::cpo_base<lu_decomposition_t>
	{
		template<typename Value,
			std::size_t RowsExtent,
			std::size_t ColumnsExtent,
			typename Allocator,
			typename To  = matrix<Value, RowsExtent, ColumnsExtent, Allocator>,
			typename To2 = matrix<Value, RowsExtent, ColumnsExtent, Allocator>>
		requires(detail::is_matrix<To>::value) friend inline auto tag_invoke(lu_decomposition_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj,
			std::type_identity<To>  = {},
			std::type_identity<To2> = {}) -> std::pair<To, To2> // @TODO: ISSUE #20
		{
			return detail::lu_impl<To, To2>(obj);
		}
	};

	inline constexpr auto lu_decomposition = lu_decomposition_t{};
} // namespace mpp
