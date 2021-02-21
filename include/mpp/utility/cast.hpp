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

#include <mpp/detail/matrix_base.hpp>
#include <mpp/detail/tag_invoke.hpp>
#include <mpp/matrix.hpp>

#include <cstddef>

namespace mpp
{
	struct cast_t
	{
		friend inline void tag_invoke(cast_t, ...) // @TODO: ISSUE #20
		{
			static_assert(R"(Custom overload of "cast" is required for custom types!)");
		}

		template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
		[[nodiscard]] friend inline auto
		tag_invoke(cast_t, std::type_identity<To>, const matrix<Value, RowsExtent, ColumnsExtent>& obj)
			-> matrix<To, RowsExtent, ColumnsExtent> // @TODO: ISSUE #20
		{
			auto casted = matrix<To, RowsExtent, ColumnsExtent>{};
			init_matrix_with_1d_rng(casted, obj, obj.rows(), obj.columns());

			return casted;
		}

		template<typename... Args>
		[[nodiscard]] auto operator()(Args&&... args) const
			-> detail::tag_invoke_impl::tag_invoke_result_t<cast_t, Args...> // @TODO: ISSUE #20
		{
			return detail::tag_invoke_cpo(*this, std::forward<Args>(args)...);
		}
	};

	inline constexpr auto cast = cast_t{};
} // namespace mpp