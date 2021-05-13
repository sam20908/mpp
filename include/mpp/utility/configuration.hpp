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

#include <array>
#include <cstddef>
#include <memory>
#include <vector>

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

		static constexpr auto use_unsafe = false;

		template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent, typename>
		using static_buffer = std::array<Value, RowsExtent * ColumnsExtent>;

		template<typename Value, std::size_t, std::size_t, typename Alloc>
		using dynamic_buffer = std::vector<Value, Alloc>;

		template<typename Value, std::size_t, std::size_t ColumnsExtent, typename Alloc>
		using dynamic_rows_buffer = dynamic_buffer<Value, 1, ColumnsExtent, Alloc>;

		template<typename Value, std::size_t RowsExtent, std::size_t, typename Alloc>
		using dynamic_columns_buffer = dynamic_buffer<Value, RowsExtent, 1, Alloc>;
	};
} // namespace mpp