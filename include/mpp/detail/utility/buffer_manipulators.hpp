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

#include <mpp/detail/types/type_traits.hpp>
#include <mpp/detail/utility/utility.hpp>

#include <cstddef>

namespace mpp::detail
{
	template<typename Buffer, typename InitializerValue>
	void allocate_buffer_if_vector(Buffer& buffer,
		std::size_t rows,
		std::size_t columns,
		InitializerValue&& val) // @TODO: ISSUE #20
	{
		constexpr auto is_vec = is_vector<Buffer>::value;

		if constexpr (is_vec)
		{
			buffer.resize(rows * columns, std::forward<InitializerValue>(val));
		}
	}

	template<typename Buffer>
	void reserve_buffer_if_vector(Buffer& buffer, std::size_t rows, std::size_t columns) // @TODO: ISSUE #20
	{
		constexpr auto is_vec = is_vector<Buffer>::value;

		if constexpr (is_vec)
		{
			buffer.reserve(rows * columns);
		}
	}

	template<typename Buffer>
	void make_identity_buffer(Buffer& buffer,
		std::size_t rows,
		std::size_t columns,
		const auto& zero_value,
		const auto& one_value) // @TODO: ISSUE #20
	{
		if constexpr (is_vector<Buffer>::value)
		{
			buffer.resize(rows * columns, zero_value);
		}

		for (auto index = std::size_t{}; index < rows; ++index)
		{
			buffer[index_2d_to_1d(columns, index, index)] = one_value;
		}
	}
} // namespace mpp::detail