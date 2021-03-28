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
#include <mpp/detail/utility/validators.hpp>

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

	template<bool ValidateDimensions, typename Buffer>
	void make_identity_buffer(Buffer& buffer,
		std::size_t rows,
		std::size_t columns,
		const auto& zero_value,
		const auto& one_value) // @TODO: ISSUE #20
	{
		if constexpr (ValidateDimensions)
		{
			validate_dimensions_for_identity_matrix(rows, columns);
		}

		const auto total_size           = rows * columns;
		constexpr auto buffer_is_vector = is_vector<Buffer>::value;

		if constexpr (buffer_is_vector)
		{
			buffer.reserve(total_size);
		}

		auto diagnoal_index = std::size_t{};
		for (auto index = std::size_t{}; index < total_size; ++index)
		{
			if constexpr (buffer_is_vector)
			{
				buffer.push_back((index == diagnoal_index) ? one_value : zero_value);
			}
			else
			{
				buffer[index] = (index == diagnoal_index) ? one_value : zero_value;
			}

			if ((index + 1) % columns == 0)
			{
				diagnoal_index += columns + 1;
			}
		}
	}
} // namespace mpp::detail