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

#include <utility>

namespace mpp::detail
{
	namespace tag_invoke_impl
	{
		void tag_invoke();

		template<typename CPO, typename... Args>
		using tag_invoke_result_t = decltype(tag_invoke(std::declval<CPO>(), std::declval<Args>()...));

		struct tag_invoke_t
		{
			template<typename CPO, typename... Args>
			[[nodiscard]] auto operator()(CPO&& cpo, Args&&... args) const
				-> tag_invoke_result_t<CPO, Args...> // @TODO: ISSUE #20
			{
				return tag_invoke(std::forward<CPO>(cpo), std::forward<Args>(args)...);
			}
		};

		// @TODO: Remove this and make tag_invoke_t constexpr when #20 is resolved
		struct tag_invoke_t_constexpr
		{
			template<typename CPO, typename... Args>
			[[nodiscard]] constexpr auto operator()(CPO&& cpo, Args&&... args) const
				-> tag_invoke_result_t<CPO, Args...>
			{
				return tag_invoke(std::forward<CPO>(cpo), std::forward<Args>(args)...);
			}
		};
	} // namespace tag_invoke_impl

	inline constexpr auto tag_invoke_cpo = detail::tag_invoke_impl::tag_invoke_t{};

	// @TODO: Remove this and make tag_invoke_t constexpr when #20 is resolved
	inline constexpr auto tag_invoke_cpo_constexpr = detail::tag_invoke_impl::tag_invoke_t_constexpr{};

} // namespace mpp::detail