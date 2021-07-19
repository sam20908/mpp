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

#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/mat.hpp>

#include <cstddef>

namespace mpp
{
	struct sq_t : public detail::cpo_base<sq_t>
	{
		template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc>
		[[nodiscard]] friend inline auto tag_invoke(sq_t,
			const mat<Val, Rows, Cols, Alloc>& obj) noexcept -> bool // @TODO: ISSUE #20
		{
			return obj.rows() == obj.cols();
		}
	};

	inline constexpr auto sq = sq_t{};
} // namespace mpp
