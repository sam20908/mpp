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
#include <mpp/mat.hpp>

#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename Mat>
		auto lu_impl(const Mat& obj) -> std::pair<To, To> // @TODO: ISSUE #20
		{
			assert(obj.rows() == obj.cols());

			using val_t = typename To::value_type;
			using buf_t = typename To::buffer_type;

			const auto rows = obj.rows();
			const auto cols = obj.cols();

			auto l = buf_t{};
			auto u = copy_into_new_buf<buf_t>(obj, rows, cols);

			init_identity_buf(l, rows, cols, val_t{}, val_t{ 1 });

			lu_algo<buf_t, true, false>(rows, cols, l, u);

			return { To{ rows, cols, std::move(l) }, To{ rows, cols, std::move(u) } };
		}
	} // namespace detail

	struct lu_t : public detail::cpo_base<lu_t>
	{
		template<typename T, typename Buf, typename To = mat<T, Buf>>
		requires(detail::is_mat<To>::value) friend inline auto tag_invoke(lu_t,
			const mat<T, Buf>& obj,
			std::type_identity<To> = {}) -> std::pair<To, To> // @TODO: ISSUE #20
		{
			return detail::lu_impl<To>(obj);
		}
	};

	inline constexpr auto lu = lu_t{};
} // namespace mpp
