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

#include <mpp/algorithm/det.hpp>
#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/cmp.hpp>
#include <mpp/mat.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace mpp
{
	struct sg_t : public detail::cpo_base<sg_t>
	{
		template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
		[[nodiscard]] friend inline auto tag_invoke(sg_t, const mat<Val, Rows, Cols, Alloc>& obj)
			-> bool // @TODO: ISSUE #20
		{
			using fp_mat_t = mat<detail::fp_t, Rows, Cols>;
			using fp_buf_t = typename fp_mat_t::buffer_type;

			const auto dummy_var = 1;
			auto obj_buf_copy    = fp_buf_t{};

			if constexpr (detail::is_vec<fp_buf_t>::value)
			{
				obj_buf_copy.reserve(obj.size());
				std::ranges::copy(obj, std::back_inserter(obj_buf_copy));
			}
			else
			{
				std::ranges::copy(obj, obj_buf_copy.begin());
			}

			const auto det_ =
				detail::lu_impl<detail::fp_t, false, true>(obj.rows(), obj.cols(), dummy_var, obj_buf_copy);

			return detail::fp_is_zero_or_nan(det_);
		}
	};

	inline constexpr auto sg = sg_t{};
} // namespace mpp
