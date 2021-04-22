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
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/comparison.hpp>
#include <mpp/matrix.hpp>

#include <compare>
#include <cstddef>

namespace mpp
{
	struct singular_t : public detail::cpo_base<singular_t>
	{
		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename Allocator>
		[[nodiscard]] friend inline auto tag_invoke(singular_t,
			const matrix<Value, RowsExtent, ColumnsExtent, Allocator>& obj) -> bool // @TODO: ISSUE #20
		{
			using ordering_type =
				std::compare_three_way_result_t<detail::default_floating_type, detail::default_floating_type>;

			return floating_point_compare(detail::det_lu_decomp<false, detail::default_floating_type>(obj),
					   detail::default_floating_type{}) == ordering_type::equivalent;
		}
	};

	inline constexpr auto singular = singular_t{};
} // namespace mpp
