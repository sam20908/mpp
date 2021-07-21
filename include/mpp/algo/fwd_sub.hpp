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

#include <mpp/detail/util/algo_impl.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/mat/matfwd.hpp>
#include <mpp/util/sq.hpp>

#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		template<typename To>
		inline auto fwd_sub_impl(const auto& a, const auto& b) -> To // @TODO: ISSUE #20
		{
			assert(sq(a));
			assert(b.cols() == 1);

			const auto rows = a.rows();

			using x_mat_t = mat_rebind_to_t<To, fp_t>;
			auto x_buf    = fwd_sub_buf<typename x_mat_t::buffer_type>(a.data(), b.data(), rows);

			return To{ rows, 1, std::move(x_buf) };
		}
	} // namespace detail

	struct fwd_sub_t : public detail::cpo_base<fwd_sub_t>
	{
		template<typename AVal,
			typename BVal,
			std::size_t ARows,
			std::size_t ACols,
			std::size_t BRows,
			std::size_t BCols,
			typename AAlloc,
			typename BAlloc,
			typename To = mat<std::common_type_t<AVal, BVal>, dyn, BCols>>
		requires(detail::is_matrix<To>::value) friend inline auto tag_invoke(fwd_sub_t,
			const mat<AVal, ARows, ACols, AAlloc>& a,
			const mat<BVal, BRows, BCols, BAlloc>& b,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::fwd_sub_impl<To>(a, b);
		}
	};

	inline constexpr auto fwd_sub = fwd_sub_t{};
} // namespace mpp
