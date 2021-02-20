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

#include <iterator>

namespace mpp_test
{
	template<typename T>
	using container_allocator_type = typename T::allocator_type;

	template<typename T>
	using container_value_type = typename T::value_type;

	template<typename T>
	using container_reference = typename T::reference;

	template<typename T>
	using container_const_reference = typename T::const_reference;

	template<typename T>
	using container_iterator = typename T::iterator;

	template<typename T>
	using container_const_iterator = typename T::const_iterator;

	template<typename T>
	using container_difference_type = typename T::difference_type;

	template<typename T>
	using container_size_type = typename T::size_type;

	template<typename Allocator>
	concept allocator_noexcept_move_constructor = requires(Allocator&& a)
	{
		{
			Allocator
			{
				std::move(a)
			}
		}
		noexcept;
	};
} // namespace mpp_test