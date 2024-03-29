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
		template<typename To>
		[[nodiscard]] inline auto inv_impl(const auto& obj) -> To // @TODO: ISSUE #20
		{
			assert(obj.rows() == obj.cols());

			const auto rows = obj.rows();
			const auto cols = obj.cols();

			using to_val_t = typename To::value_type;
			using to_buf_t = typename To::buffer_type;

			// Handle special cases - avoid LU Decomposition
			if (rows == 0)
			{
				return To{};
			}

			auto buf = copy_into_new_buf<to_buf_t>(obj, rows, cols);

			if (rows == 1)
			{
				const auto elem = static_cast<to_val_t>(obj(0, 0));

				assert(!is_zero_or_nan(elem));

				buf[0] = 1 / elem;
			}

			if (rows == 2)
			{
				const auto a = static_cast<to_val_t>(obj(1, 1));
				const auto b = static_cast<to_val_t>(obj(0, 1));
				const auto c = static_cast<to_val_t>(obj(1, 0));
				const auto d = static_cast<to_val_t>(obj(0, 0));

				const auto ad = a * d;
				const auto bc = b * c;

				const auto det_ = ad - bc;

				assert(!is_zero_or_nan(det_));

				const auto inv_det = 1 / det_;

				buf[0] = inv_det * a;
				buf[1] = inv_det * b * -1;
				buf[2] = inv_det * c * -1;
				buf[3] = inv_det * d;
			}

			if (rows >= 3)
			{
				// @FIXME: We can't use to_buf_t since it'll fail for containers such as std::array because we can't
				// change their size
				using subst_buf_t = std::vector<to_val_t>;
				auto l            = subst_buf_t{};
				auto u            = copy_into_new_buf<subst_buf_t>(obj, rows, cols);
				init_identity_buf(l, rows, cols, to_val_t{}, to_val_t{ 1 });

				const auto det_ = lu_algo<to_val_t, true, true>(rows, cols, l, u);
				assert(!is_zero_or_nan(det_));

				auto x_buf = subst_buf_t{};
				resize_buf_if_dyn(x_buf, rows, 1, to_val_t{});

				// Solve for x_buffer values with Ax=b where A=l and b=Column of identity matrix

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
					x_buf[last_col_idx] = to_val_t{};
					x_buf[row]          = to_val_t{ 1 };

					auto l_x_buf = fwd_sub_buf<subst_buf_t>(l, x_buf, rows);

					// Use l_x_buf to do back substitution to solve Ax=B with A=u and b=l_x_buf. The
					// inv_col now corresponds to a column of the inverse matrix

					auto inv_col = back_sub_buf<subst_buf_t>(u, std::move(l_x_buf), rows);

					for (auto col = std::size_t{}; auto val : inv_col)
					{
						buf[idx_1d(cols, col++, row)] = val;
					}
				}
			}

			return To{ rows, cols, std::move(buf) };
		}
	} // namespace detail

	struct inv_t : public detail::cpo_base<inv_t>
	{
		template<typename T, typename Buf, typename To = mat<T, Buf>>
		requires(detail::is_mat<To>::value)
			[[nodiscard]] friend inline auto tag_invoke(inv_t, const mat<T, Buf>& obj, std::type_identity<To> = {})
				-> To // @TODO: ISSUE #20
		{
			return detail::inv_impl<To>(obj);
		}
	};

	inline constexpr auto inv = inv_t{};
} // namespace mpp
