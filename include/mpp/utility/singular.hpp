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

#include <mpp/algorithm/determinant.hpp>
#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace mpp
{
	struct singular_t : public detail::cpo_base<singular_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
		[[nodiscard]] friend inline auto tag_invoke(singular_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj) -> bool // @TODO: ISSUE #20
		{
			using fp_matrix_t = matrix<detail::default_floating_type, RowsExtent, ColumnsExtent>;
			using fp_buffer_t = typename fp_matrix_t::buffer_type;

			const auto dummy_l_buffer = 1;
			auto obj_buf_copy         = fp_buffer_t{};

			if constexpr (detail::is_vector<fp_buffer_t>::value)
			{
				obj_buf_copy.reserve(obj.size());
				std::ranges::copy(obj, std::back_inserter(obj_buf_copy));
			}
			else
			{
				std::ranges::copy(obj, obj_buf_copy.begin());
			}

			const auto det = detail::lu_generic<detail::default_floating_type, false, true>(obj.rows(),
				obj.columns(),
				dummy_l_buffer,
				obj_buf_copy);

			return detail::fp_is_zero_or_nan(det);
		}
	};

	inline constexpr auto singular = singular_t{};
} // namespace mpp
