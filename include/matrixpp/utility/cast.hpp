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

#include "../detail/matrix_base.hpp"
#include "../matrix.hpp"

#include <cstddef>

namespace matrixpp
{
	template<typename To, typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline decltype(auto) cast(const matrix<Value, RowsExtent, ColumnsExtent>& obj) // @TODO: ISSUE #20
	{
		using casted_matrix_t = matrix<To, RowsExtent, ColumnsExtent>;
		auto casted           = casted_matrix_t{};

		detail::init_matrix_base_with_1d_rng(casted, obj.buffer(), obj.rows(), obj.columns());

		return casted;
	}
} // namespace matrixpp