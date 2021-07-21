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

#include <mpp/detail/util/tag_invoke.hpp>

namespace mpp::detail
{
	template<typename CPO>
	struct cpo_base
	{
		template<typename... Args>
		[[nodiscard]] auto operator()(Args&&... args) const
			noexcept(noexcept(tag_invoke_cpo(CPO{}, std::forward<Args>(args)...)))
				-> tag_invoke_result_t<CPO, Args...> // @TODO: ISSUE #20
		{
			// Practically empty CPO objects gets optimized out, so it's okay to create it to help overload resolution
			return tag_invoke_cpo(CPO{}, std::forward<Args>(args)...);
		}
	};
} // namespace mpp::detail