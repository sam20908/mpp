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
	struct cfg
	{
		template<typename Val>
		using alloc = std::allocator<Val>;

		static constexpr auto rows_extent = dyn;
		static constexpr auto cols_extent = dyn;

		template<typename Val, std::size_t Rows, std::size_t Cols, typename>
		using fixed_buf = std::array<Val, Rows * Cols>;

		template<typename Val, std::size_t, std::size_t, typename Alloc>
		using dyn_buf = std::vector<Val, Alloc>;

		template<typename Val, std::size_t, std::size_t Cols, typename Alloc>
		using dyn_rows_buf = dyn_buf<Val, 1, Cols, Alloc>;

		template<typename Val, std::size_t Rows, std::size_t, typename Alloc>
		using dyn_cols_buf = dyn_buf<Val, Rows, 1, Alloc>;
	};
} // namespace mpp