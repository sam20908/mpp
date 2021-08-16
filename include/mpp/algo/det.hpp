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

#include <algorithm>
#include <cassert>

namespace mpp
{
	namespace detail
	{
		template<typename To, typename Mat>
		[[nodiscard]] inline auto det_impl(const Mat& obj) -> To // @TODO: ISSUE #20
		{
			assert(obj.rows() == obj.cols());

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
				const auto ad = obj(0, 0) * obj(1, 1);
				const auto bc = obj(0, 1) * obj(1, 0);

				return static_cast<To>(ad - bc);
			}

			auto buf = obj.buffer();

			// The determinant of a LU Decomposition is det(A) = det(L) * det(U) Since det(L) is always 1, we can avoid
			// creating L entirely
			const auto det_ = lu_algo<To, false, true>(rows, cols, dummy, buf);

			return det_;
		}
	} // namespace detail

	struct det_t : public detail::cpo_base<det_t>
	{
		template<typename T, typename Buf, typename To = T>
		requires(std::is_arithmetic_v<To>)
			[[nodiscard]] friend inline auto tag_invoke(det_t, const mat<T, Buf>& obj, std::type_identity<To> = {})
				-> To // @TODO: ISSUE #20
		{
			return detail::det_impl<To>(obj);
		}
	};

	inline constexpr auto det = det_t{};
} // namespace mpp
