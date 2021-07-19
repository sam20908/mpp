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
#include <mpp/detail/utility/buffer_manipulators.hpp>
#include <mpp/detail/utility/cpo_base.hpp>
#include <mpp/utility/sq.hpp>
#include <mpp/mat.hpp>

#include <cassert>
#include <cmath>
#include <memory>

namespace mpp
{
	namespace detail
	{
		inline static constexpr auto dummy_variable = ' ';

		template<typename To, typename Mat>
		[[nodiscard]] inline auto det_impl(const Mat& obj) -> To // @TODO: ISSUE #20
		{
			assert(sq(obj));

			const auto rows = obj.rows();
			const auto cols = obj.cols();

			// Handle special cases - avoid computing LU Decomposition
			if (rows == 0)
			{
				return To{ 1 };
			}
			else if (rows == 1)
			{
				return static_cast<To>(obj(0, 0));
			}
			else if (rows == 2)
			{
				const auto ad = static_cast<fp_t>(obj(0, 0)) * static_cast<fp_t>(obj(1, 1));
				const auto bc = static_cast<fp_t>(obj(0, 1)) * static_cast<fp_t>(obj(1, 0));

				const auto result = ad - bc;

				return static_cast<To>(result);
			}

			using lu_buf_t = typename mat_rebind_to_t<Mat, fp_t>::buffer_type;

			auto u = lu_buf_t{};

			// If the incoming matrix has an array as its buffer, we can just use the same type of buffer since it'll be
			// less overhead and we know it's the same size

			resize_buf_if_vec(u, rows, cols, fp_t{});
			std::ranges::copy(obj, u.begin());

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U) Since det(L) is always 1, we can avoid
			// creating L entirely
			const auto det_ = lu_impl<fp_t, false, true>(rows, cols, dummy_variable, u);

			// @FIXME: Is it correct to just directly cast?
			return static_cast<To>(det_);
		}
	} // namespace detail

	struct det_t : public detail::cpo_base<det_t>
	{
		template<typename Val, std::size_t Rows, std::size_t Cols, typename Alloc, typename To = Val>
		requires(std::is_arithmetic_v<To>) [[nodiscard]] friend inline auto tag_invoke(det_t,
			const mat<Val, Rows, Cols, Alloc>& obj,
			std::type_identity<To> = {}) -> To // @TODO: ISSUE #20
		{
			return detail::det_impl<To>(obj);
		}
	};

	inline constexpr auto det = det_t{};
} // namespace mpp
