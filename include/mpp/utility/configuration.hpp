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

// @TODO: Export std::allocator for modules
#include <mpp/detail/utility/public.hpp>

#include <cstddef>
#include <memory>

namespace mpp
{
	struct override;

	/**
	 * Defines the global configuration the library uses
	 */
	template<typename>
	struct configuration
	{
		template<typename Value>
		using allocator = std::allocator<Value>;

		static constexpr auto rows_extent    = dynamic;
		static constexpr auto columns_extent = dynamic;
	};
} // namespace mpp