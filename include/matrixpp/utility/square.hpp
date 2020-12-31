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

#include "../matrix.hpp"

#include <cstddef>

namespace matrixpp
{
	template<typename Value, std::size_t RowsExtent, std::size_t ColumnsExtent>
	[[nodiscard]] inline auto square(const matrix<Value, RowsExtent, ColumnsExtent>& obj) // @TODO: ISSUE #20
	{
		return obj.rows() == obj.columns();
	}
} // namespace matrixpp