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
#include <mpp/detail/util/cpo_base.hpp>
#include <mpp/mat.hpp>

#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		template<typename To>
		inline auto back_sub_impl(const auto& a, const auto& b) -> To // @TODO: ISSUE #20
		{
			assert(a.rows() == a.cols());
			assert(b.cols() == 1);

			const auto rows = a.rows();
			auto buf        = back_sub_buf<typename To::buffer_type>(a.data(), b.data(), rows);

			return To{ rows, 1, std::move(buf) };
		}
	} // namespace detail

	struct back_sub_t : public detail::cpo_base<back_sub_t>
	{
		template<typename AT, typename BT, typename ABuf, typename BBuf, typename To = mat<std::common_type_t<AT, BT>>>
		requires(detail::is_mat<To>::value) friend inline auto tag_invoke(back_sub_t,
			const mat<AT, ABuf>& a,
			const mat<BT, BBuf>& b,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::back_sub_impl<To>(a, b);
		}
	};

	inline constexpr auto back_sub = back_sub_t{};
} // namespace mpp
