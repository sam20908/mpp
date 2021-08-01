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
#include <mpp/detail/util/util.hpp>
#include <mpp/mat/matfwd.hpp>
#include <mpp/util/sq.hpp>

#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename To2, typename Mat>
		auto lu_impl(const Mat& obj) -> std::pair<To, To2> // @TODO: ISSUE #20
		{
			assert(sq(obj));

			using lu_buffer_t = typename mat_rebind_to_t<Mat, fp_t>::buffer_type;

			const auto rows = obj.rows();
			const auto cols = obj.cols();

			auto l = lu_buffer_t{};
			auto u = lu_buffer_t{};

			// @TODO: Should do a direct buffer copy initialization instead
			resize_buf_if_vec(u, rows, cols, fp_t{});
			std::ranges::copy(obj, u.begin());

			init_identity_buf(l, rows, cols, fp_t{}, fp_t{ 1 });

			lu_impl<fp_t, true, false>(rows, cols, l, u);

			return { To{ rows, cols, std::move(l) }, To2{ rows, cols, std::move(u) } };
		}
	} // namespace detail

	struct lu_t : public detail::cpo_base<lu_t>
	{
		template<typename Val,
			std::size_t Rows,
			std::size_t Cols,
			typename Alloc,
			typename To  = mat<Val, Rows, Cols, Alloc>,
			typename To2 = mat<Val, Rows, Cols, Alloc>>
		requires(detail::is_mat<To>::value&& detail::is_mat<To2>::value) friend inline auto tag_invoke(lu_t,
			const mat<Val, Rows, Cols, Alloc>& obj,
			std::type_identity<To>  = {},
			std::type_identity<To2> = {}) -> std::pair<To, To2> // @TODO: ISSUE #20
		{
			return detail::lu_impl<To, To2>(obj);
		}
	};

	inline constexpr auto lu = lu_t{};
} // namespace mpp
