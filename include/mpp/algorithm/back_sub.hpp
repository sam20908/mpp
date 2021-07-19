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

#include <mpp/detail/types/algo_types.hpp>
#include <mpp/detail/utility/algorithm_helpers.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/cmp.hpp>
#include <mpp/utility/sq.hpp>
#include <mpp/mat.hpp>

#include <cassert>
#include <memory>
#include <utility>

namespace mpp
{
	namespace detail
	{
		template<typename Buf>
		inline auto back_sub_buf(const auto& a, const auto& b, std::size_t n) -> Buf // @TODO: ISSUE #20
		{
			auto x_buf = Buf{};

			// @TODO: Any way to make this utilize push_back?
			resize_buf_if_vec(x_buf, n, 1, fp_t{});

			/**
			 * Implementation of back substitution from
			 * https://www.gaussianwaves.com/2013/05/solving-a-triangular-matrix-using-back-backward-substitution/
			 */
			for (auto row = n; row > std::size_t{}; --row)
			{
				const auto row_idx = row - 1;

				auto res = static_cast<fp_t>(b[idx_1d(1, row_idx, 0)]);

				for (auto col = n - 1; col > row_idx; --col)
				{
					res -= a[idx_1d(n, row_idx, col)] * x_buf[col];
				}

				const auto diag = static_cast<fp_t>(a[idx_1d(n, row_idx, row_idx)]);

				assert(!fp_is_zero_or_nan(diag));

				res /= diag;

				x_buf[row_idx] = res;
			}

			return x_buf;
		}

		template<typename To>
		inline auto back_sub_impl(const auto& a, const auto& b) -> To // @TODO: ISSUE #20
		{
			assert(sq(a));
			assert(b.cols() == 1);

			const auto rows = a.rows();

			using x_mat_t = mat_rebind_to_t<To, fp_t>;
			auto x_buf    = back_sub_buf<typename x_mat_t::buffer_type>(a.data(), b.data(), rows);

			return To{ rows, 1, std::move(x_buf) };
		}
	} // namespace detail

	struct back_sub_t : public detail::cpo_base<back_sub_t>
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
		requires(detail::is_matrix<To>::value) friend inline auto tag_invoke(back_sub_t,
			const mat<AVal, ARows, ACols, AAlloc>& a,
			const mat<BVal, BRows, BCols, BAlloc>& b,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::back_sub_impl<To>(a, b);
		}
	};

	inline constexpr auto back_sub = back_sub_t{};
} // namespace mpp
