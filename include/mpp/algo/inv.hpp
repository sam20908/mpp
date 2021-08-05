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

#include <cassert>
#include <cstddef>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename Mat>
		[[nodiscard]] inline auto inv_impl(const Mat& obj) -> To // @TODO: ISSUE #20
		{
			assert(obj.rows() == obj.cols());

			const auto rows = obj.rows();
			const auto cols = obj.cols();

			using lu_buf_t = typename mat_rebind_to_t<Mat, fp_t>::buffer_type;

			// Handle special cases - avoid LU Decomposition
			if (rows == 0)
			{
				return To{};
			}

			auto inv_buf = lu_buf_t{};

			// If the incoming matrix has an array as its buffer, we can just use the same type of buffer since it'll be
			// less overhead and we know it's the same size

			resize_buf_if_vec(inv_buf, rows, cols, fp_t{});

			if (rows == 1)
			{
				const auto elem = static_cast<fp_t>(obj(0, 0));

				assert(!is_zero_or_nan(elem));

				inv_buf[0] = 1 / elem;
			}

			if (rows == 2)
			{
				const auto a = static_cast<fp_t>(obj(1, 1));
				const auto b = static_cast<fp_t>(obj(0, 1));
				const auto c = static_cast<fp_t>(obj(1, 0));
				const auto d = static_cast<fp_t>(obj(0, 0));

				const auto ad = a * d;
				const auto bc = b * c;

				const auto det_ = ad - bc;

				assert(!is_zero_or_nan(det_));

				const auto inv_det = 1 / det_;

				inv_buf[0] = inv_det * a;
				inv_buf[1] = inv_det * b * -1;
				inv_buf[2] = inv_det * c * -1;
				inv_buf[3] = inv_det * d;
			}

			if (rows >= 3)
			{
				auto l = lu_buf_t{};
				auto u = lu_buf_t{};

				// @TODO: Should do a direct buffer copy initialization instead
				resize_buf_if_vec(u, rows, cols, fp_t{});
				std::ranges::copy(obj, u.begin());

				init_identity_buf(l, rows, cols, fp_t{}, fp_t{ 1 });

				const auto det_ = lu_impl<fp_t, true, true>(rows, cols, l, u);

				assert(!is_zero_or_nan(det_));

				// Solve for x_buffer values with Ax=b where A=l and b=Column of identity matrix

				using x_buf_t = typename mat<fp_t, Mat::rows_extent(), 1>::buffer_type;
				auto x        = x_buf_t{};

				resize_buf_if_vec(x, rows, 1, fp_t{});

				for (auto row = std::size_t{}; row < rows; ++row)
				{
					// Branch-less logic to do:
					// row if row == 0
					// row - 1 if row > 0
					const auto last_col_idx = row - 1 * (row != 0);

					// This forms b that corresponds to corresponding column from the identity matrix
					// e.g. with a 3x1 column vector
					// row 1 -> 1 0 0
					// row 2 -> 0 1 0
					// row 3 -> 0 0 1
					x[last_col_idx] = fp_t{};
					x[row]          = fp_t{ 1 };

					auto l_x_buffer = fwd_sub_buf<x_buf_t>(l, x, rows);

					// Use l_x_buffer to do back substitution to solve Ax=B with A=u and b=l_x_buffer. The
					// inv_col now corresponds to a column of the inverse matrix

					auto inv_col = back_sub_buf<x_buf_t>(u, std::move(l_x_buffer), rows);

					for (auto col = std::size_t{}; auto val : inv_col)
					{
						inv_buf[idx_1d(cols, col++, row)] = val;
					}
				}
			}

			return To{ rows, cols, std::move(inv_buf) };
		}
	} // namespace detail

	struct inv_t : public detail::cpo_base<inv_t>
	{
		template<typename Val,
			std::size_t Rows,
			std::size_t Cols,
			typename Alloc,
			typename To = mat<Val, Rows, Cols, Alloc>>
		requires(detail::is_mat<To>::value) [[nodiscard]] friend inline auto tag_invoke(inv_t,
			const mat<Val, Rows, Cols, Alloc>& obj,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::inv_impl<To>(obj);
		}
	};

	inline constexpr auto inv = inv_t{};
} // namespace mpp
