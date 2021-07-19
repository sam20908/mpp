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
	template<typename Buf>
	void resize_buf_if_vec(Buf& buf,
		std::size_t rows,
		std::size_t cols,
		auto val) // @TODO: ISSUE #20
	{
		if constexpr (is_vec<Buf>::value)
		{
			buf.resize(rows * cols, val);
		}
	}

	template<typename Buf>
	void reserve_buf_if_vec(Buf& buffer, std::size_t rows, std::size_t cols) // @TODO: ISSUE #20
	{
		if constexpr (is_vec<Buf>::value)
		{
			buffer.reserve(rows * cols);
		}
	}

	template<typename Buf>
	void init_identity_buf(Buf& buffer,
		std::size_t rows,
		std::size_t cols,
		auto zero_value,
		auto one_value) // @TODO: ISSUE #20
	{
		if constexpr (is_vec<Buf>::value)
		{
			buffer.resize(rows * cols, zero_value);
		}

		for (auto index = std::size_t{}; index < rows; ++index)
		{
			buffer[idx_1d(cols, index, index)] = one_value;
		}
	}
} // namespace mpp::detail