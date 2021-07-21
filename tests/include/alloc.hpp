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

#include <cstddef>
#include <cstdlib>
#include <limits>
#include <memory>

/**
 * Taken from https://en.cppreference.com/w/cpp/named_req/Allocator#Allocator_completeness_requirements with
 * modifications
 */

template<class T>
struct custom_allocator
{
	typedef T value_type;

	custom_allocator() = default;
	template<class U>
	constexpr custom_allocator(const custom_allocator<U>&) noexcept
	{
	}

	[[nodiscard]] T* allocate(std::size_t n)
	{
		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_alloc();

		if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
		{
			return p;
		}

		throw std::bad_alloc();
	}

	void deallocate(T* p, std::size_t) noexcept
	{
		std::free(p);
	}
};

template<class T, class U>
bool operator==(const custom_allocator<T>&, const custom_allocator<U>&)
{
	return true;
}
template<class T, class U>
bool operator!=(const custom_allocator<T>&, const custom_allocator<U>&)
{
	return false;
}
