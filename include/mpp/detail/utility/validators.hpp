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

#include <mpp/detail/utility/exception_messages.hpp>

#include <cstddef>
#include <stdexcept>

namespace mpp::detail
{
	inline void validate_matrices_same_size(const auto& left, const auto& right) // @TODO: ISSUE #20
	{
		if (left.rows() != right.rows() || left.columns() != right.columns())
		{
			throw std::invalid_argument(DIMENSIONS_NOT_THE_SAME);
		}
	}
	inline void validate_matrices_multipliable(const auto& left, const auto& right) // @TODO: ISSUE #20
	{
		if (left.columns() != right.rows())
		{
			throw std::invalid_argument(DIMENSIONS_NOT_MULTIPLIABLE);
		}
	}

	inline void validate_dimensions_square(const auto& obj) // @TODO: ISSUE #20
	{
		if (obj.rows() != obj.rows())
		{
			throw std::invalid_argument(DIMENSIONS_NOT_THE_SAME);
		}
	}

	inline void validate_dimensions_for_identity_matrix(std::size_t rows, std::size_t columns) // @TODO: ISSUE #20
	{
		if (rows != columns)
		{
			throw std::invalid_argument(DIMENSIONS_NOT_SQUARE);
		}

		if (rows == 0 || columns == 0)
		{
			throw std::invalid_argument(IDENTITY_RANK_OF_ZERO);
		}
	}
} // namespace mpp::detail